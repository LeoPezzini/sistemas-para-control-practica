#define RS485_TX 17
#define RS485_RX 16
#define RS485_DIR 4

HardwareSerial RS485(2);

unsigned long contador = 0;

void setup() {
  Serial.begin(115200);

  pinMode(RS485_DIR, OUTPUT);

  // Inicialmente recepción
  digitalWrite(RS485_DIR, LOW);

  RS485.begin(
    9600,
    SERIAL_8N1,
    RS485_RX,
    RS485_TX
  );

  Serial.println("=== TEST RS485 ===");
}

void loop() {

  // Habilitar transmisor
  digitalWrite(RS485_DIR, HIGH);

  delayMicroseconds(100);

  RS485.print("HOLA RS485 ");
  RS485.println(contador++);

  // Esperar a que realmente termine la transmisión
  RS485.flush();

  delayMicroseconds(100);

  // Volver a recepción
  digitalWrite(RS485_DIR, LOW);

  Serial.println("Mensaje enviado por RS485");

  delay(1000);
}