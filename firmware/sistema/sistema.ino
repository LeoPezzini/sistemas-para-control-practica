#include <dhtESP32-rmt.h>
#include <ModbusRTU.h>

// ============================================================
// PINES
// ============================================================

// DHT11
#define DHT_PIN      27

// HC-SR04
#define TRIG_PIN     25
#define ECHO_PIN     26

// RS485 / MAX485
#define RS485_RX     16
#define RS485_TX     17
#define RS485_DIR    4

// Encoder Omron E6B2-CWZ6C 1000 P/R
#define ENCODER_A    32
#define ENCODER_B    33

// ============================================================
// OBJETOS
// ============================================================

HardwareSerial RS485(2);
ModbusRTU mb;

// ============================================================
// REGISTROS MODBUS
// ============================================================
//
// HR0 = Temperatura [°C] x10
// HR1 = Humedad [%] x10
// HR2 = Distancia sin compensar [cm] x10
// HR3 = Distancia compensada por temperatura [cm] x10
//
// HR4 = Contador encoder - palabra alta
// HR5 = Contador encoder - palabra baja
// HR6 = Posición angular [°] x10
//

const uint16_t REG_TEMP       = 0;
const uint16_t REG_HUM        = 1;
const uint16_t REG_DIST_RAW   = 2;
const uint16_t REG_DIST_COMP  = 3;

const uint16_t REG_ENCODER_HI = 4;
const uint16_t REG_ENCODER_LO = 5;
const uint16_t REG_ANGULO     = 6;

// ============================================================
// VARIABLES DHT11
// ============================================================

float temperatura = 0.0f;
float humedad = 0.0f;

bool temperaturaValida = false;

// ============================================================
// VARIABLES HC-SR04
// ============================================================

float distanciaSinCompensar = 0.0f;
float distanciaCompensada = 0.0f;

// ============================================================
// VARIABLES ENCODER
// ============================================================

// El encoder es de 1000 P/R.
//
// Como estamos generando una interrupción en CHANGE sobre la fase A,
// detectamos flanco ascendente + flanco descendente:
//
// 1000 pulsos/rev x 2 flancos = 2000 cuentas/rev
//
const int32_t CUENTAS_POR_VUELTA = 2000;

volatile int32_t contadorEncoder = 0;

// ============================================================
// TEMPORIZACIÓN
// ============================================================

unsigned long ultimoDHT = 0;
unsigned long ultimoUltrasonido = 0;
unsigned long ultimaImpresionEncoder = 0;

const unsigned long PERIODO_DHT = 3000;
const unsigned long PERIODO_ULTRASONIDO = 500;
const unsigned long PERIODO_PRINT_ENCODER = 500;

// ============================================================
// INTERRUPCIÓN DEL ENCODER
// ============================================================

void IRAM_ATTR encoderISR() {

  bool A = digitalRead(ENCODER_A);
  bool B = digitalRead(ENCODER_B);

  // Esta convención coincide con la prueba realizada:
  //
  // antihorario -> aumenta
  // horario     -> disminuye

  if (A == B) {
    contadorEncoder++;
  }
  else {
    contadorEncoder--;
  }
}

// ============================================================
// HC-SR04
// ============================================================

/*
 * Genera el pulso TRIG y devuelve el tiempo de vuelo
 * medido por ECHO en microsegundos.
 *
 * Devuelve 0 si no se recibe eco.
 */
unsigned long medirTiempoEco() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  return pulseIn(ECHO_PIN, HIGH, 30000);
}


/*
 * Distancia usando velocidad fija:
 *
 * c = 343 m/s
 *   = 0.0343 cm/us
 */
float calcularDistanciaSinCompensar(
  unsigned long tiempo_us
) {

  const float VELOCIDAD_SONIDO_FIJA =
      0.0343f; // cm/us

  return tiempo_us *
         VELOCIDAD_SONIDO_FIJA /
         2.0f;
}


/*
 * Velocidad del sonido compensada por temperatura.
 *
 * c(T) = 331.3 + 0.606*T
 *
 * c en m/s
 * T en °C
 */
float calcularVelocidadSonido(
  float temperaturaC
) {

  return 331.3f +
         0.606f * temperaturaC;
}


/*
 * Distancia ultrasónica compensada por temperatura.
 */
float calcularDistanciaCompensada(
  unsigned long tiempo_us,
  float temperaturaC
) {

  // Velocidad en m/s
  float velocidad_ms =
      calcularVelocidadSonido(
        temperaturaC
      );

  // Conversión:
  // m/s -> cm/us
  float velocidad_cm_us =
      velocidad_ms / 10000.0f;

  // El tiempo corresponde a ida + vuelta
  return tiempo_us *
         velocidad_cm_us /
         2.0f;
}

// ============================================================
// ESCALADO MODBUS
// ============================================================

/*
 * Convierte un float a entero x10.
 *
 * Ejemplos:
 *
 * 24.0 °C  -> 240
 * 12.9 cm  -> 129
 * 180.5 °  -> 1805
 */
uint16_t escalarX10(float valor) {

  return (uint16_t)(
    valor * 10.0f + 0.5f
  );
}

// ============================================================
// POSICIÓN ANGULAR
// ============================================================

float calcularAngulo(
  int32_t contador
) {

  /*
   * Nos quedamos con la posición dentro
   * de una vuelta.
   *
   * El resultado queda entre:
   *
   * 0° <= ángulo < 360°
   */

  int32_t cuentasModulo =
      contador % CUENTAS_POR_VUELTA;

  // En C/C++ el módulo puede quedar negativo.
  // Lo corregimos para obtener siempre 0...1999.
  if (cuentasModulo < 0) {
    cuentasModulo += CUENTAS_POR_VUELTA;
  }

  float angulo =
      ((float)cuentasModulo /
       (float)CUENTAS_POR_VUELTA)
      * 360.0f;

  return angulo;
}

// ============================================================
// SETUP
// ============================================================

void setup() {

  Serial.begin(115200);

  delay(1000);

  // ----------------------------------------------------------
  // HC-SR04
  // ----------------------------------------------------------

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(TRIG_PIN, LOW);

  // ----------------------------------------------------------
  // ENCODER
  // ----------------------------------------------------------

  pinMode(ENCODER_A, INPUT);
  pinMode(ENCODER_B, INPUT);

  attachInterrupt(
    digitalPinToInterrupt(ENCODER_A),
    encoderISR,
    CHANGE
  );

  // ----------------------------------------------------------
  // RS485
  // ----------------------------------------------------------

  RS485.begin(
    9600,
    SERIAL_8N1,
    RS485_RX,
    RS485_TX
  );

  // DE y /RE del MAX485 unidos en GPIO4
  mb.begin(&RS485, RS485_DIR);

  // ESP32 = Modbus Slave ID 3 para integración en bus común
  mb.slave(3);

  // ----------------------------------------------------------
  // REGISTROS MODBUS
  // ----------------------------------------------------------

  mb.addHreg(REG_TEMP, 0);
  mb.addHreg(REG_HUM, 0);

  mb.addHreg(REG_DIST_RAW, 0);
  mb.addHreg(REG_DIST_COMP, 0);

  mb.addHreg(REG_ENCODER_HI, 0);
  mb.addHreg(REG_ENCODER_LO, 0);
  mb.addHreg(REG_ANGULO, 0);

  // ----------------------------------------------------------
  // INFORMACIÓN
  // ----------------------------------------------------------

  Serial.println();
  Serial.println(
    "======================================"
  );

  Serial.println(
    " SISTEMA INTEGRADO - ESP32"
  );

  Serial.println(
    "======================================"
  );

  Serial.println();
  Serial.println(
    "Modbus RTU Slave ID: 3"
  );

  Serial.println();
  Serial.println(
    "Mapa de registros:"
  );

  Serial.println(
    "HR0 = Temperatura x10"
  );

  Serial.println(
    "HR1 = Humedad x10"
  );

  Serial.println(
    "HR2 = Distancia sin compensar x10"
  );

  Serial.println(
    "HR3 = Distancia compensada x10"
  );

  Serial.println(
    "HR4 = Encoder contador HIGH"
  );

  Serial.println(
    "HR5 = Encoder contador LOW"
  );

  Serial.println(
    "HR6 = Posicion angular x10"
  );

  Serial.println();
}

// ============================================================
// LOOP
// ============================================================

void loop() {

  // ==========================================================
  // MODBUS
  // ==========================================================

  // Debe ejecutarse continuamente.
  mb.task();

  unsigned long ahora = millis();

  // ==========================================================
  // DHT11
  // ==========================================================

  if (
    ahora - ultimoDHT >=
    PERIODO_DHT
  ) {

    ultimoDHT = ahora;

    uint8_t error =
        read_dht(
          temperatura,
          humedad,
          DHT_PIN,
          DHT11
        );

    if (error == DHT_OK) {

      temperaturaValida = true;

      mb.Hreg(
        REG_TEMP,
        escalarX10(temperatura)
      );

      mb.Hreg(
        REG_HUM,
        escalarX10(humedad)
      );

      Serial.println(
        "------------------------------"
      );

      Serial.print(
        "Temperatura: "
      );

      Serial.print(
        temperatura,
        1
      );

      Serial.println(
        " °C"
      );

      Serial.print(
        "Humedad: "
      );

      Serial.print(
        humedad,
        1
      );

      Serial.println(
        " %"
      );

      float velocidad =
          calcularVelocidadSonido(
            temperatura
          );

      Serial.print(
        "Velocidad sonido: "
      );

      Serial.print(
        velocidad,
        2
      );

      Serial.println(
        " m/s"
      );
    }

    else {

      Serial.print(
        "ERROR DHT: "
      );

      Serial.println(
        error
      );
    }
  }

  // ==========================================================
  // HC-SR04
  // ==========================================================

  if (
    ahora - ultimoUltrasonido >=
    PERIODO_ULTRASONIDO
  ) {

    ultimoUltrasonido = ahora;

    unsigned long tiempoEco =
        medirTiempoEco();

    if (tiempoEco > 0) {

      // -------------------------------------------------------
      // Distancia SIN compensar
      // -------------------------------------------------------

      distanciaSinCompensar =
          calcularDistanciaSinCompensar(
            tiempoEco
          );

      mb.Hreg(
        REG_DIST_RAW,
        escalarX10(
          distanciaSinCompensar
        )
      );

      // -------------------------------------------------------
      // Distancia COMPENSADA
      // -------------------------------------------------------

      if (temperaturaValida) {

        distanciaCompensada =
            calcularDistanciaCompensada(
              tiempoEco,
              temperatura
            );

        mb.Hreg(
          REG_DIST_COMP,
          escalarX10(
            distanciaCompensada
          )
        );
      }

      // -------------------------------------------------------
      // Monitor Serie
      // -------------------------------------------------------

      Serial.print(
        "Tiempo eco: "
      );

      Serial.print(
        tiempoEco
      );

      Serial.println(
        " us"
      );

      Serial.print(
        "Distancia sin compensar: "
      );

      Serial.print(
        distanciaSinCompensar,
        2
      );

      Serial.println(
        " cm"
      );

      if (temperaturaValida) {

        Serial.print(
          "Distancia compensada: "
        );

        Serial.print(
          distanciaCompensada,
          2
        );

        Serial.println(
          " cm"
        );
      }

      else {

        Serial.println(
          "Distancia compensada: "
          "esperando temperatura valida"
        );
      }

      Serial.println();
    }

    else {

      Serial.println(
        "ERROR HC-SR04: "
        "no se recibio eco"
      );
    }
  }

  // ==========================================================
  // ENCODER
  // ==========================================================

  /*
   * Como contadorEncoder se modifica dentro de una ISR,
   * realizamos una copia atómica antes de utilizarlo.
   */

  noInterrupts();

  int32_t contadorActual =
      contadorEncoder;

  interrupts();

  // ----------------------------------------------------------
  // Contador de 32 bits -> dos Holding Registers de 16 bits
  // ----------------------------------------------------------

  uint16_t encoderHi =
      (uint16_t)(
        (contadorActual >> 16)
        & 0xFFFF
      );

  uint16_t encoderLo =
      (uint16_t)(
        contadorActual
        & 0xFFFF
      );

  mb.Hreg(
    REG_ENCODER_HI,
    encoderHi
  );

  mb.Hreg(
    REG_ENCODER_LO,
    encoderLo
  );

  // ----------------------------------------------------------
  // Posición angular
  // ----------------------------------------------------------

  float angulo =
      calcularAngulo(
        contadorActual
      );

  mb.Hreg(
    REG_ANGULO,
    escalarX10(angulo)
  );

  // ----------------------------------------------------------
  // Monitor Serie del encoder
  // ----------------------------------------------------------

  if (
    ahora - ultimaImpresionEncoder >=
    PERIODO_PRINT_ENCODER
  ) {

    ultimaImpresionEncoder = ahora;

    Serial.print(
      "Encoder: "
    );

    Serial.print(
      contadorActual
    );

    Serial.print(
      " cuentas | Angulo: "
    );

    Serial.print(
      angulo,
      1
    );

    Serial.println(
      " grados"
    );
  }

  // ==========================================================

  delay(1);
}