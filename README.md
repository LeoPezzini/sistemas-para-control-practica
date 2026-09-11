# Sistemas para Control

Repositorio correspondiente a la práctica de la asignatura **Sistemas para Control**.

## Descripción

Desarrollo de un sistema de adquisición, procesamiento, comunicación, supervisión y registro de variables utilizando un microcontrolador ESP32.

El sistema contempla:

- adquisición de temperatura y humedad;
- medición de distancia mediante ultrasonido;
- compensación de la medición ultrasónica por temperatura;
- caracterización del sensor ultrasónico contra un instrumento patrón;
- adquisición de posición mediante encoder incremental;
- comunicación Modbus RTU sobre RS485;
- supervisión mediante Rapid SCADA;
- visualización mediante Webstation desde PC y dispositivos de la red local;
- registro histórico de temperatura y humedad;
- generación de reportes históricos en formato Excel.

## Integrantes

- Leonardo Pezzini
- Federico Cappato

---

## Arquitectura

Se utiliza un **ESP32 NodeMCU como nodo principal de adquisición y procesamiento**.

La utilización del ESP32 NodeMCU fue consultada y aceptada por la cátedra.

La arquitectura implementada es:

```text
DHT11 ───────────────┐
                     │
HC-SR04 ─────────────┼──► ESP32 NodeMCU
                     │
Encoder incremental ─┘
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
             ┌─────────────┴─────────────┐
             │                           │
        Modbus Poll                 Rapid SCADA
                                         │
                            ┌────────────┴────────────┐
                            │                         │
                         Server                  Webstation
                            │                         │
                       Histórico               Navegador web
                         Sec30                 PC / celular
```

El ESP32 funciona como **esclavo Modbus RTU ID 1** y la PC actúa como maestro.

Modbus Poll fue utilizado inicialmente para verificar la comunicación y el mapa de registros. En la implementación SCADA definitiva, **Rapid SCADA Communicator actúa como maestro Modbus RTU**.

---

## Hardware

### ESP32 NodeMCU

Nodo principal de adquisición, procesamiento y comunicación.

Asignación de GPIO:

| Dispositivo | Señal | GPIO | Estado |
|---|---|---:|---|
| DHT11 | DATA | 27 | Verificado |
| HC-SR04 | TRIG | 25 | Verificado |
| HC-SR04 | ECHO | 26 | Verificado |
| MAX485 | TX / DI | 17 | Verificado |
| MAX485 | RX / RO | 16 | Verificado |
| MAX485 | DE + /RE | 4 | Verificado |
| Encoder | Canal A | 32 | Verificado |
| Encoder | Canal B | 33 | Verificado |

### Waveshare ESP32-S3-POE-ETH-8DI-8DO

Plataforma provista inicialmente por la cátedra.

Durante las primeras pruebas se evaluó su utilización para adquisición directa de sensores.

Las entradas digitales de la placa corresponden a interfaces industriales optoaisladas y no a GPIO convencionales de 3.3 V, por lo que no resultaron adecuadas para determinados sensores utilizados en la práctica.

También se realizaron pruebas mediante GPIO directos disponibles en la placa.

Finalmente se decidió utilizar un **ESP32 NodeMCU como nodo principal de adquisición**, decisión aceptada por la cátedra.

---

## Comunicación RS485

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

La salida RO del MAX485 se adapta mediante un divisor resistivo:

- 1 kΩ entre RO y GPIO16;
- 2 kΩ entre GPIO16 y GND.

Durante las pruebas se midieron aproximadamente:

- RO: 4.57 V;
- GPIO16: 3.18 V.

La comunicación física fue verificada en ambos sentidos antes de implementar Modbus RTU.

---

## Sensores

### DHT11

Sensor utilizado para medir temperatura y humedad relativa.

**Estado: verificado funcionalmente.**

Configuración:

- alimentación: 3.3 V;
- DATA: GPIO27;
- librería: `dhtESP32-rmt`.

La temperatura medida también se utiliza para compensar la medición realizada mediante ultrasonido.

---

### HC-SR04

Sensor ultrasónico utilizado para medir distancia.

**Estado: verificado funcionalmente.**

Configuración:

- alimentación: 5 V;
- TRIG: GPIO25;
- ECHO: GPIO26 mediante divisor resistivo 1 kΩ / 2 kΩ.

La señal ECHO se adapta a aproximadamente 3.3 V antes de ingresar al ESP32.

#### Compensación por temperatura

La distancia ultrasónica depende de la velocidad de propagación del sonido.

Se utiliza la aproximación:

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

El firmware conserva simultáneamente:

- distancia calculada utilizando una velocidad fija de 343 m/s;
- distancia compensada utilizando la temperatura medida por el DHT11.

Ambos valores se publican mediante Modbus para permitir su comparación.

#### Caracterización

La caracterización definitiva está prevista utilizando el medidor patrón **UNI-T LM50A** provisto por la cátedra.

Se prevén mediciones en incrementos de 10 cm dentro del rango definido para el ensayo.

Para cada punto se registrarán:

- distancia patrón;
- temperatura;
- distancia ultrasónica sin compensación;
- distancia ultrasónica compensada;
- error respecto del patrón.

Posteriormente se realizará la curva de error correspondiente.

**Estado: pendiente de ensayo con instrumento patrón.**

---

### Encoder incremental

Se utiliza un encoder óptico incremental **Omron E6B2-CWZ6C de 1000 P/R**.

Configuración:

- alimentación: 5 V;
- salida: NPN open collector;
- fase A: GPIO32 con pull-up externo de 4.7 kΩ a 3.3 V;
- fase B: GPIO33 con pull-up externo de 4.7 kΩ a 3.3 V;
- fase Z: no utilizada en la implementación actual.

La adquisición se realiza mediante interrupciones.

En la implementación actual se detectan ambos flancos de la fase A (`CHANGE`) y se utiliza el estado de la fase B para determinar el sentido de giro.

Dado que el encoder posee 1000 P/R:

```text
1000 pulsos/revolución × 2 flancos = 2000 cuentas/revolución
```

Por lo tanto:

```text
Resolución angular = 360° / 2000 = 0.18° por cuenta
```

La posición angular se expresa entre 0° y 360° respecto de la posición existente al iniciar el sistema.

Al tratarse de un encoder incremental, el sistema no conoce una posición absoluta al encenderse. La fase Z no se utiliza actualmente como referencia de origen.

Se verificó:

- incremento del contador en sentido antihorario;
- decremento del contador en sentido horario;
- estabilidad con el eje detenido;
- retorno aproximadamente a 0° luego de una revolución completa hasta la marca física inicial.

---

## Modbus RTU

El ESP32 funciona como:

**Modbus RTU Slave ID 1**

La PC funciona como maestro Modbus.

La comunicación fue verificada inicialmente utilizando Modbus Poll y posteriormente mediante Rapid SCADA.

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
| HR4 | Contador encoder - palabra alta | 16 bits |
| HR5 | Contador encoder - palabra baja | 16 bits |
| HR6 | Posición angular [°] | ×10 |

Los siete registros se leen mediante:

**Function Code 03 - Read Holding Registers**

La lectura se realiza como un bloque contiguo de siete Holding Registers.

Inicialmente se verificó Modbus utilizando un registro de prueba con valor fijo `1234`. Posteriormente se incorporaron todas las variables adquiridas.

El contador del encoder es de 32 bits con signo y se transmite mediante dos Holding Registers de 16 bits:

- HR4: palabra alta;
- HR5: palabra baja.

---

## Rapid SCADA

Rapid SCADA se utiliza como sistema de supervisión y adquisición.

La arquitectura implementada es:

```text
ESP32 Slave ID 1
      │
   Modbus RTU
      │
    RS485
      │
USB-RS485 / COM8
      │
      ▼
Rapid SCADA Communicator
      │
      ▼
Rapid SCADA Server
      │
      ├──► Archivo histórico
      │
      └──► Webstation
```

### Communicator

Se configuró la línea:

```text
Bus RS485
```

Configuración del canal:

- tipo: Serial Port;
- puerto: COM8;
- baud rate: 9600;
- data bits: 8;
- parity: None;
- stop bits: 1;
- comportamiento: Master.

Dispositivo:

- nombre: `ESP32 NodeMCU`;
- código: `ESP32_01`;
- protocolo: Modbus RTU;
- driver: `DrvModbus`;
- dirección Modbus: 1;
- timeout: 1000 ms;
- delay: 200 ms.

Se configuró una lectura mediante Function Code 03 de los siete registros del ESP32.

La comunicación fue verificada mediante el log de Communicator, observándose peticiones equivalentes a:

```text
01 03 00 00 00 07 ...
```

correspondientes a:

- Slave ID 1;
- Function Code 03;
- dirección inicial 0;
- cantidad 7 registros.

El dispositivo permanece en estado `Normal` y el polling se realiza sin errores.

---

## Canales de Rapid SCADA

Se crearon los siguientes canales:

| Canal | Tag | Variable |
|---:|---|---|
| 101 | TEMP | Temperatura |
| 102 | HUM | Humedad |
| 103 | DIST_RAW | Distancia sin compensación |
| 104 | DIST_COMP | Distancia compensada |
| 105 | ENC_HIGH | Encoder palabra alta |
| 106 | ENC_LOW | Encoder palabra baja |
| 107 | ANGULO | Posición angular |

Para las variables transmitidas escaladas ×10 se utiliza la fórmula de entrada:

```text
Cnl * 0.1
```

La fórmula se aplica a:

- canal 101;
- canal 102;
- canal 103;
- canal 104;
- canal 107.

Los canales 105 y 106 conservan las palabras de 16 bits del contador del encoder sin aplicar dicha escala.

---

## Webstation

Se creó una vista de tabla:

```text
ESP32.tbl
```

La vista permite supervisar:

- temperatura;
- humedad;
- distancia sin compensar;
- distancia compensada;
- contador del encoder;
- posición angular.

Se verificó experimentalmente que la modificación física de la distancia y el movimiento del encoder producen la actualización correspondiente en Webstation.

Webstation se encuentra disponible en el puerto TCP:

```text
10008
```

Se verificó el acceso desde:

- navegador de la PC;
- navegador de un teléfono conectado a la misma red local.

Para permitir el acceso desde otros dispositivos se configuró una regla de entrada en Windows Firewall para TCP 10008.

---

## Registro histórico

Para cumplir el requisito de registrar temperatura y humedad con período de 30 segundos se creó un archivo histórico personalizado.

Configuración:

```text
Code: Sec30
Name: 30 Second Archive
Kind: Historical
Module: ModArcBasic
Writing period: 30 seconds
```

En la base de configuración se asignó al archivo personalizado el **bit 16**.

Los canales incluidos en este archivo son:

- 101 - Temperatura;
- 102 - Humedad.

No se almacenan en `Sec30` distancia ni posición angular, ya que el registro histórico de 24 horas corresponde específicamente a temperatura y humedad.

La vista `ESP32.tbl` fue configurada para consultar:

```text
Archive code: Sec30
Chart arguments: archive=Sec30
```

### Verificación del período

El período real de almacenamiento fue comprobado mediante un reporte histórico exportado a Microsoft Excel.

Se observaron timestamps consecutivos:

```text
11:10:00
11:10:30
11:11:00
11:11:30
11:12:00
11:12:30
...
```

Por lo tanto, se verificó experimentalmente un período de almacenamiento de **30 segundos**.

Para una adquisición continua de 24 horas se esperan aproximadamente:

```text
24 × 60 × 60 / 30 = 2880 muestras por variable
```

La generación y exportación del reporte Excel fue verificada.

**Pendiente:** realizar la adquisición definitiva durante 24 horas.

---

## Comunicación validada

Se verificaron experimentalmente:

1. transmisión ESP32 → PC mediante RS485;
2. recepción PC → ESP32 mediante RS485;
3. comunicación Modbus RTU;
4. lectura mediante Modbus Poll;
5. adquisición simultánea DHT11 + HC-SR04;
6. compensación térmica del HC-SR04;
7. adquisición del encoder mediante interrupciones;
8. integración encoder + Modbus;
9. lectura HR0-HR6 mediante Rapid SCADA;
10. visualización dinámica mediante Webstation;
11. registro histórico cada 30 segundos;
12. generación de reporte Excel;
13. acceso a Webstation desde un dispositivo móvil de la red local.

---

## Software

- Arduino IDE
- ESP32 Arduino Core
- `dhtESP32-rmt`
- `modbus-esp8266 / ModbusRTU`
- Modbus Poll
- Rapid SCADA
- PuTTY
- Git
- GitHub

---

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
| Encoder incremental | ✅ |
| Integración encoder + Modbus | ✅ |
| Rapid SCADA / Modbus RTU | ✅ |
| Canales y escalado SCADA | ✅ |
| Webstation | ✅ |
| Acceso desde dispositivo móvil | ✅ |
| Archivo histórico de 30 s | ✅ |
| Exportación a Excel | ✅ |
| Caracterización HC-SR04 con LM50A | ⏳ |
| Registro definitivo de 24 h | ⏳ |
| Prueba/coord. de bus RS485 compartido | ⏳ |
| Comando de posición relativa al robot | ❓ Aclarar con cátedra |
| Informe y documentación final | ⏳ |