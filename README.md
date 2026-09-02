# Sistemas para Control

Repositorio correspondiente a la práctica de la asignatura **Sistemas para Control**.

## Descripción

Desarrollo de un sistema de adquisición, procesamiento, comunicación y supervisión de variables utilizando microcontroladores ESP32.

El sistema contempla:

- adquisición de temperatura y humedad;
- medición de distancia mediante ultrasonido;
- adquisición mediante encoder incremental;
- comunicación Modbus RTU sobre RS485;
- supervisión mediante RapidSCADA;
- registro y análisis de variables.

Actualmente se encuentra en evaluación una arquitectura basada en un **ESP32 NodeMCU como nodo de adquisición**, debido a dificultades encontradas al utilizar GPIO directos de la placa Waveshare ESP32-S3-POE-ETH-8DI-8DO para determinados sensores.

## Integrantes

- Leonardo Pezzini
- Federico Cappato

## Hardware

### ESP32 NodeMCU

Utilizado actualmente para las pruebas de adquisición y comunicación.

Asignación verificada:

| Dispositivo | Señal | GPIO |
|---|---|---:|
| DHT11 | DATA | 27 |
| HC-SR04 | TRIG | 25 |
| HC-SR04 | ECHO | 26 |

### Waveshare ESP32-S3-POE-ETH-8DI-8DO

Plataforma provista por la cátedra.

Su utilización definitiva dentro de la arquitectura se encuentra pendiente de definición.

### Comunicación RS485

Para las pruebas se utiliza:

- módulo MAX485 alimentado a 5 V;
- adaptación de nivel en la salida RO mediante divisor resistivo;
- conversor USB-RS485 basado en CH340;
- UART2 del ESP32 NodeMCU.

Asignación propuesta:

| MAX485 | ESP32 |
|---|---|
| DI | GPIO17 / TX2 |
| RO | GPIO16 / RX2 mediante divisor |
| DE + /RE | GPIO4 |
| VCC | 5 V |
| GND | GND |

## Sensores

### DHT11

Sensor de temperatura y humedad.

Estado: **verificado funcionalmente**.

Configuración actual:

- alimentación: 3.3 V;
- DATA: GPIO27;
- librería: `dhtESP32-rmt`.

Lecturas preliminares obtenidas:

- temperatura: aproximadamente 23 °C;
- humedad relativa: aproximadamente 18 %.

### HC-SR04

Sensor ultrasónico de distancia.

Estado: **verificado funcionalmente**.

Configuración actual:

- alimentación: 5 V;
- TRIG: GPIO25;
- ECHO: GPIO26 mediante divisor resistivo 1 kΩ / 2 kΩ.

La comparación preliminar con cinta métrica mostró una diferencia aproximada de 1 cm.

La caracterización y calibración definitiva se realizará posteriormente utilizando el patrón provisto por la cátedra.

### Encoder

Pendiente de especificación y entrega de información por parte de la cátedra.

## Comunicación

Se verificó correctamente la comunicación:

ESP32 NodeMCU → MAX485 → RS485 → USB-RS485 → PC

La prueba consistió en el envío periódico de texto desde UART2 del ESP32 y su recepción en la PC mediante PuTTY.

Configuración utilizada:

- 9600 baud;
- 8 bits de datos;
- sin paridad;
- 1 bit de stop.

Próximo objetivo:

implementar un esclavo **Modbus RTU** en el ESP32 y verificar su lectura mediante Modbus Poll.

## Software

- Arduino IDE
- ESP32 Arduino Core
- Modbus Poll
- RapidSCADA
- PuTTY
- Git / GitHub

## Estado del proyecto

| Etapa | Estado |
|---|---|
| Configuración ESP32 NodeMCU | ✅ |
| Blink ESP32 | ✅ |
| DHT11 | ✅ |
| HC-SR04 | ✅ |
| Comunicación UART → RS485 → PC | ✅ |
| Encoder | ⏳ |
| Modbus RTU | ⏳ |
| Integración de sensores | ⏳ |
| RapidSCADA | ⏳ |
| Caracterización HC-SR04 | ⏳ |
| Registro de variables | ⏳ |
| Documentación final | ⏳ |