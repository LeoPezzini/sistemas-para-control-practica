# Sistemas para Control

Repositorio correspondiente a la práctica de la asignatura **Sistemas para Control**.

## Descripción

Desarrollo de un sistema de adquisición, procesamiento, comunicación y supervisión de variables utilizando un microcontrolador ESP32.

El sistema contempla:

- adquisición de temperatura y humedad;
- medición de distancia mediante ultrasonido;
- compensación de la medición ultrasónica por temperatura;
- caracterización del sensor ultrasónico contra un instrumento patrón;
- adquisición mediante encoder incremental;
- comunicación Modbus RTU sobre RS485;
- supervisión mediante RapidSCADA;
- registro y análisis de variables.

## Integrantes

- Leonardo Pezzini
- Federico Cappato

## Arquitectura

Se utiliza un **ESP32 NodeMCU como nodo principal de adquisición y procesamiento**.

La utilización del ESP32 NodeMCU fue consultada y aceptada por la cátedra.

Arquitectura actual:

```text
DHT11 ──────────────┐
                    │
HC-SR04 ────────────┼──► ESP32 NodeMCU
                    │
Encoder ────────────┘
                         │
                       UART2
                         │
                         ▼
                       MAX485
                         │
                       RS485
                         │
                         ▼
                     USB-RS485
                         │
                         ▼
                         PC
                         │
                    Modbus Poll
                         │
                    RapidSCADA
```

## Hardware

### ESP32 NodeMCU

Nodo principal de adquisición, procesamiento y comunicación.

Asignación actual de GPIO:

| Dispositivo | Señal | GPIO | Estado |
|---|---|---:|---|
| DHT11 | DATA | 27 | Verificado |
| HC-SR04 | TRIG | 25 | Verificado |
| HC-SR04 | ECHO | 26 | Verificado |
| MAX485 | TX / DI | 17 | Verificado |
| MAX485 | RX / RO | 16 | Verificado |
| MAX485 | DE + /RE | 4 | Verificado |
| Encoder | Canal A | - | Pendiente |
| Encoder | Canal B | - | Pendiente |

### Waveshare ESP32-S3-POE-ETH-8DI-8DO

Plataforma provista inicialmente por la cátedra.

Durante las primeras pruebas se evaluó su utilización para adquisición directa de sensores.

Las entradas digitales de la placa corresponden a interfaces industriales optoaisladas y no a GPIO convencionales de 3.3 V, por lo que no resultaron adecuadas para determinados sensores utilizados en la práctica.

También se realizaron pruebas mediante GPIO directos disponibles en la placa.

Finalmente se decidió utilizar un ESP32 NodeMCU como nodo principal de adquisición, decisión aceptada por la cátedra.

### Comunicación RS485

Se utiliza:

- módulo MAX485 alimentado a 5 V;
- adaptación de nivel en la salida RO mediante divisor resistivo;
- conversor USB-RS485 basado en CH340;
- UART2 del ESP32 NodeMCU.

Asignación:

| MAX485 | ESP32 |
|---|---|
| DI | GPIO17 / TX2 |
| RO | GPIO16 / RX2 mediante divisor |
| DE + /RE | GPIO4 |
| VCC | 5 V |
| GND | GND |

La salida RO del MAX485 se adapta mediante un divisor resistivo de:

- 1 kΩ entre RO y GPIO16;
- 2 kΩ entre GPIO16 y GND.

Durante las pruebas se midieron aproximadamente:

- RO: 4.57 V;
- GPIO16: 3.18 V.

## Sensores

### DHT11

Sensor utilizado para medir temperatura y humedad relativa.

Estado: **verificado funcionalmente**.

Configuración:

- alimentación: 3.3 V;
- DATA: GPIO27;
- librería: `dhtESP32-rmt`.

La temperatura medida también se utiliza para compensar la medición realizada mediante ultrasonido.

### HC-SR04

Sensor ultrasónico utilizado para medir distancia.

Estado: **verificado funcionalmente**.

Configuración:

- alimentación: 5 V;
- TRIG: GPIO25;
- ECHO: GPIO26 mediante divisor resistivo 1 kΩ / 2 kΩ.

La señal ECHO se adapta a aproximadamente 3.3 V antes de ingresar al ESP32.

#### Compensación por temperatura

La distancia ultrasónica depende de la velocidad de propagación del sonido.

Como aproximación se utiliza:

```text
c(T) = 331.3 + 0.606 T
```

donde:

- `c` es la velocidad del sonido en m/s;
- `T` es la temperatura ambiente en °C.

El tiempo medido por el HC-SR04 corresponde al recorrido de ida y vuelta, por lo que:

```text
d = t · c(T) / 2
```

El firmware conserva tanto la distancia calculada utilizando una velocidad fija de 343 m/s como la distancia compensada por temperatura.

#### Caracterización

La caracterización definitiva se realizará utilizando el medidor patrón **UNI-T LM50A** provisto por la cátedra.

Se realizarán mediciones en incrementos de 10 cm dentro del rango definido para el ensayo.

Para cada punto se registrarán:

- distancia patrón;
- temperatura;
- distancia ultrasónica sin compensación;
- distancia ultrasónica compensada;
- error respecto del patrón.

Posteriormente se realizará la curva de error correspondiente.

### Encoder

Pendiente de especificación e integración.

## Modbus RTU

El ESP32 funciona como:

**Modbus RTU Slave ID 1**

La PC funciona como Modbus Master.

La comunicación fue verificada utilizando Modbus Poll mediante RS485.

Configuración:

- 9600 baud;
- 8 bits de datos;
- sin paridad;
- 1 bit de stop;
- modo RTU.

### Mapa de registros

| Registro | Variable | Escala |
|---:|---|---:|
| HR0 | Temperatura [°C] | ×10 |
| HR1 | Humedad relativa [%] | ×10 |
| HR2 | Distancia sin compensación [cm] | ×10 |
| HR3 | Distancia compensada por temperatura [cm] | ×10 |

Los registros se leen mediante:

**Function Code 03 - Read Holding Registers**

Inicialmente se verificó Modbus utilizando un registro de prueba con valor fijo `1234`.

Posteriormente se integraron las variables adquiridas por los sensores.

## Comunicación validada

Se verificaron independientemente:

1. transmisión ESP32 → PC mediante RS485;
2. recepción PC → ESP32 mediante RS485;
3. comunicación Modbus RTU bidireccional;
4. lectura de registros Modbus;
5. integración de DHT11 y HC-SR04 con Modbus RTU.

Arquitectura validada:

```text
Sensores
   ↓
ESP32
   ↓
UART2
   ↓
MAX485
   ↓
RS485
   ↓
USB-RS485
   ↓
PC
   ↓
Modbus Poll
```

## Software

- Arduino IDE
- ESP32 Arduino Core
- `dhtESP32-rmt`
- `modbus-esp8266 / ModbusRTU`
- Modbus Poll
- RapidSCADA
- PuTTY
- Git
- GitHub

## Estado del proyecto

| Etapa | Estado |
|---|---|
| Configuración ESP32 NodeMCU | ✅ |
| DHT11 | ✅ |
| HC-SR04 | ✅ |
| RS485 | ✅ |
| Modbus RTU | ✅ |
| Integración sensores + Modbus | ✅ |
| Compensación por temperatura | ✅ |
| Encoder | ⏳ |
| Caracterización HC-SR04 | ⏳ |
| RapidSCADA | ⏳ |
| Registro de variables | ⏳ |
| Documentación final | ⏳ |