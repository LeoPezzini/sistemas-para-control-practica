#include <dhtESP32-rmt.h>
#include <ModbusRTU.h>

// -------------------- Pines --------------------

#define DHT_PIN      27

#define TRIG_PIN     25
#define ECHO_PIN     26

#define RS485_RX     16
#define RS485_TX     17
#define RS485_DIR    4

// -------------------- Objetos --------------------

HardwareSerial RS485(2);
ModbusRTU mb;

// -------------------- Variables --------------------

float temperatura = 0.0;
float humedad = 0.0;
float distancia_cm = 0.0;

// -------------------- Registros Modbus --------------------
//
// HR0 = temperatura x10
// HR1 = humedad x10
// HR2 = distancia x10
//

const uint16_t REG_TEMP = 0;
const uint16_t REG_HUM  = 1;
const uint16_t REG_DIST = 2;

// -------------------- Temporización --------------------

unsigned long ultimoDHT = 0;
unsigned long ultimoUltrasonido = 0;

const unsigned long PERIODO_DHT = 3000;
const unsigned long PERIODO_ULTRASONIDO = 500;

// ------------------------------------------------------------

float medirDistancia() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duracion =
      pulseIn(ECHO_PIN, HIGH, 30000);

  if (duracion == 0) {
    return -1.0;
  }

  return duracion * 0.0343f / 2.0f;
}

// ------------------------------------------------------------

void setup() {

  Serial.begin(115200);

  // HC-SR04
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);

  // RS485
  RS485.begin(
    9600,
    SERIAL_8N1,
    RS485_RX,
    RS485_TX
  );

  mb.begin(&RS485, RS485_DIR);
  mb.slave(1);

  // Registros Modbus
  mb.addHreg(REG_TEMP, 0);
  mb.addHreg(REG_HUM, 0);
  mb.addHreg(REG_DIST, 0);

  Serial.println();
  Serial.println("=== SISTEMA INTEGRADO ===");
  Serial.println("ESP32 Modbus RTU Slave ID 1");

  Serial.println("HR0 = Temperatura x10");
  Serial.println("HR1 = Humedad x10");
  Serial.println("HR2 = Distancia x10");
}

// ------------------------------------------------------------

void loop() {

  // Modbus debe atenderse continuamente
  mb.task();

  unsigned long ahora = millis();

  // ---------------- DHT11 ----------------

  if (ahora - ultimoDHT >= PERIODO_DHT) {

    ultimoDHT = ahora;

    uint8_t error = read_dht(
      temperatura,
      humedad,
      DHT_PIN,
      DHT11
    );

    if (error == DHT_OK) {

      uint16_t temp_modbus =
          (uint16_t)(temperatura * 10.0f);

      uint16_t hum_modbus =
          (uint16_t)(humedad * 10.0f);

      mb.Hreg(REG_TEMP, temp_modbus);
      mb.Hreg(REG_HUM, hum_modbus);

      Serial.print("T: ");
      Serial.print(temperatura, 1);

      Serial.print(" C | HR: ");
      Serial.print(humedad, 1);

      Serial.println(" %");

    } else {

      Serial.print("ERROR DHT: ");
      Serial.println(error);
    }
  }

  // ---------------- HC-SR04 ----------------

  if (ahora - ultimoUltrasonido >= PERIODO_ULTRASONIDO) {

    ultimoUltrasonido = ahora;

    float nuevaDistancia =
        medirDistancia();

    if (nuevaDistancia >= 0.0) {

      distancia_cm = nuevaDistancia;

      uint16_t dist_modbus =
          (uint16_t)(distancia_cm * 10.0f);

      mb.Hreg(REG_DIST, dist_modbus);

      Serial.print("Distancia: ");
      Serial.print(distancia_cm, 1);
      Serial.println(" cm");

    } else {

      Serial.println("ERROR HC-SR04: sin eco");
    }
  }

  delay(1);
}