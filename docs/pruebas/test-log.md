\## TEST-002 — DHT11



\*\*Objetivo:\*\*  

Verificar la adquisición de temperatura y humedad mediante un sensor DHT11 conectado directamente al ESP32-S3.



\*\*Conexión:\*\*

\- VCC → 3,3 V

\- GND → GND lógico

\- DATA → GPIO1



\*\*Software:\*\*

\- Plataforma: ESP32-S3

\- Librería utilizada: dhtESP32-rmt

\- Período de lectura: 5 s



\*\*Resultados:\*\*

\- Comunicación con el sensor: correcta.

\- Temperatura estabilizada observada: aproximadamente 24 °C.

\- Humedad estabilizada observada: aproximadamente 16–21 %.

\- El sensor respondió correctamente ante cambios de humedad provocados por aire exhalado.

\- El sensor respondió ante perturbaciones térmicas.

\- Se observaron algunos errores de comunicación durante perturbaciones térmicas rápidas.



\*\*Prueba de referencia:\*\*

El mismo módulo DHT11 fue probado con un Arduino UNO alimentado a 5 V, obteniéndose lecturas correctas.



\*\*Observaciones:\*\*

La librería DHT utilizada inicialmente no permitió obtener lecturas válidas en el ESP32-S3. La lectura mediante `dhtESP32-rmt`, basada en el periférico RMT del ESP32, funcionó correctamente.



La exactitud absoluta de las mediciones no fue verificada con instrumental calibrado.



\*\*Estado:\*\* APROBADO

## TEST-003 - DHT11 sobre ESP32 NodeMCU

- GPIO DATA: 27
- Alimentación: 3.3 V
- Resultado observado: ~23 °C, ~18 % HR
- Estado: APROBADO

## TEST-004 - HC-SR04 sobre ESP32 NodeMCU

- TRIG: GPIO25
- ECHO: GPIO26
- ECHO adaptado mediante divisor 1 kΩ / 2 kΩ
- Alimentación: 5 V
- Comparación preliminar mediante cinta métrica
- Diferencia observada: aproximadamente 1 cm
- Calibración definitiva: pendiente de patrón de cátedra
- Estado: APROBADO FUNCIONALMENTE

