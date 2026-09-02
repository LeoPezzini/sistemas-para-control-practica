#define TRIG_PIN 25
#define ECHO_PIN 26

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(TRIG_PIN, LOW);

  delay(1000);
  Serial.println("=== HC-SR04 - ESP32 NodeMCU ===");
}

void loop() {

  // Asegurar pulso limpio
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);

  // Pulso de disparo de 10 us
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Medir duración del pulso ECHO
  unsigned long duracion = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duracion == 0) {
    Serial.println("ERROR: no se recibio eco");
  }
  else {
    // Velocidad del sonido aproximada: 343 m/s
    float distancia_cm = duracion * 0.0343f / 2.0f;

    Serial.print("Tiempo: ");
    Serial.print(duracion);
    Serial.print(" us | Distancia: ");
    Serial.print(distancia_cm, 1);
    Serial.println(" cm");
  }

  delay(500);
}