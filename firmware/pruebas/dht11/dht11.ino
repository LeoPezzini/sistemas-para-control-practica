#include <dhtESP32-rmt.h>

#define DHT_PIN 1

float temperatura = 0.0;
float humedad = 0.0;

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("=== PRUEBA DHT11 CON RMT ===");
}

void loop() {
  uint8_t error = read_dht(
    temperatura,
    humedad,
    DHT_PIN,
    DHT11
  );

  if (error == DHT_OK) {
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println(" C");

    Serial.print("Humedad: ");
    Serial.print(humedad);
    Serial.println(" %");
  }
  else {
    Serial.print("Error DHT: ");
    Serial.println(error);
  }

  Serial.println();
  delay(5000);
}