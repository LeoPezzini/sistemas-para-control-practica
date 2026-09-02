#include <dhtESP32-rmt.h>

#define DHT_PIN 27

float temperatura = 0.0;
float humedad = 0.0;

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println();
  Serial.println("=== DHT11 - ESP32 NodeMCU ===");
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
    Serial.print(temperatura, 1);
    Serial.print(" C | Humedad: ");
    Serial.print(humedad, 1);
    Serial.println(" %");
  }
  else {
    Serial.print("ERROR DHT: ");
    Serial.println(error);
  }

  delay(3000);
}