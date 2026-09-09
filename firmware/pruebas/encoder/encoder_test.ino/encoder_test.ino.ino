#define ENCODER_A 32
#define ENCODER_B 33

volatile long contador = 0;

void IRAM_ATTR encoderISR() {

  bool A = digitalRead(ENCODER_A);
  bool B = digitalRead(ENCODER_B);

  if (A == B) {
    contador++;
  } else {
    contador--;
  }
}

void setup() {

  Serial.begin(115200);

  pinMode(ENCODER_A, INPUT);
  pinMode(ENCODER_B, INPUT);

  attachInterrupt(
    digitalPinToInterrupt(ENCODER_A),
    encoderISR,
    CHANGE
  );

  delay(500);

  Serial.println();
  Serial.println("=== TEST ENCODER E6B2-CWZ6C ===");
  Serial.println("Resolucion nominal: 1000 P/R");
}

void loop() {

  static long ultimoValor = 0;

  noInterrupts();
  long valor = contador;
  interrupts();

  if (valor != ultimoValor) {

    Serial.print("Contador: ");
    Serial.println(valor);

    ultimoValor = valor;
  }

  delay(20);
}