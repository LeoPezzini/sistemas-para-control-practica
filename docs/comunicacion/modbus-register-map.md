# Mapa de registros Modbus

## Configuración general

El ESP32 NodeMCU funciona como esclavo Modbus RTU y publica las variables adquiridas mediante Holding Registers.

La configuración implementada es:

- protocolo: Modbus RTU;
- dispositivo: ESP32 NodeMCU;
- rol del ESP32: Slave;
- Slave ID: 1;
- rol de Rapid SCADA: Master;
- baud rate: 9600 bit/s;
- bits de datos: 8;
- paridad: ninguna;
- bits de stop: 1;
- formato: 8N1;
- función de lectura: Function Code 03 - Read Holding Registers.

## Holding Registers

El mapa implementado contiene siete registros contiguos, desde HR0 hasta HR6.

| Dirección | Registro | Variable | Unidad | Representación |
|---:|---|---|---|---|
| 0 | HR0 | Temperatura | °C | valor ×10 |
| 1 | HR1 | Humedad relativa | % | valor ×10 |
| 2 | HR2 | Distancia sin compensación | cm | valor ×10 |
| 3 | HR3 | Distancia compensada por temperatura | cm | valor ×10 |
| 4 | HR4 | Contador encoder HIGH | - | palabra alta de 16 bits |
| 5 | HR5 | Contador encoder LOW | - | palabra baja de 16 bits |
| 6 | HR6 | Posición angular | ° | valor ×10 |

Los siete registros se leen como un único bloque mediante FC03.

La petición del maestro corresponde conceptualmente a:

```text
Slave ID:          1
Function Code:     03
Dirección inicial: 0
Cantidad:          7 registros
```

Durante las pruebas con Rapid SCADA se observaron peticiones con la estructura:

```text
01 03 00 00 00 07 ...
```

donde:

```text
01       → dirección del esclavo
03       → Read Holding Registers
00 00    → dirección inicial 0
00 07    → cantidad de registros solicitados
...      → CRC Modbus RTU
```

## Representación de variables decimales

Los Holding Registers utilizados son de 16 bits y las variables de temperatura, humedad, distancia y posición angular requieren conservar una cifra decimal.

Para evitar transmitir valores de punto flotante, el ESP32 multiplica estas variables por 10 antes de almacenarlas en los registros.

Por ejemplo:

```text
Temperatura real = 24.0 °C
Valor transmitido en HR0 = 240
```

En Rapid SCADA se aplica posteriormente:

```text
Cnl * 0.1
```

por lo que:

```text
240 × 0.1 = 24.0 °C
```

Este procedimiento se utiliza en:

- HR0 - temperatura;
- HR1 - humedad;
- HR2 - distancia sin compensación;
- HR3 - distancia compensada;
- HR6 - posición angular.

## Representación del contador del encoder

El contador del encoder se almacena internamente en el ESP32 como un entero de 32 bits con signo.

Como un Holding Register contiene 16 bits, el contador se divide en dos registros:

```text
HR4 = palabra alta
HR5 = palabra baja
```

Conceptualmente:

```text
Contador de 32 bits
┌───────────────────────────────┐
│       HIGH       │     LOW    │
│      16 bits     │   16 bits  │
└───────────────────────────────┘
        │                │
        ▼                ▼
       HR4              HR5
```

Los canales correspondientes a HR4 y HR5 se conservan principalmente para verificación del contador interno.

La posición angular utilizada para supervisión se publica directamente en HR6.

## Relación entre registros Modbus y canales Rapid SCADA

La correspondencia implementada es:

| Registro | Canal SCADA | Tag | Variable | Fórmula SCADA |
|---|---:|---|---|---|
| HR0 | 101 | TEMP | Temperatura | `Cnl * 0.1` |
| HR1 | 102 | HUM | Humedad relativa | `Cnl * 0.1` |
| HR2 | 103 | DIST_RAW | Distancia sin compensación | `Cnl * 0.1` |
| HR3 | 104 | DIST_COMP | Distancia compensada | `Cnl * 0.1` |
| HR4 | 105 | ENC_HIGH | Encoder palabra alta | sin escala |
| HR5 | 106 | ENC_LOW | Encoder palabra baja | sin escala |
| HR6 | 107 | ANGULO | Posición angular | `Cnl * 0.1` |

De esta manera, la cadena de representación de una variable es:

```text
Variable física
      │
      ▼
    ESP32
      │
escalado ×10
      │
      ▼
Holding Register
      │
 Modbus RTU / FC03
      │
      ▼
Rapid SCADA
      │
 fórmula ×0.1
      │
      ▼
Variable en unidades físicas
```

## Configuración en Rapid SCADA

En Communicator se configuró el dispositivo:

```text
Nombre: ESP32 NodeMCU
Código: ESP32_01
Protocolo: Modbus RTU
Dirección: 1
```

La línea serie utilizada durante las pruebas fue:

```text
Puerto: COM8
Baud rate: 9600
Data bits: 8
Parity: None
Stop bits: 1
Modo: Master
```

El template Modbus realiza una lectura de siete elementos `ushort` comenzando en la dirección lógica correspondiente al primer Holding Register.

## Uso de Modbus Poll

Modbus Poll se utilizó como herramienta de diagnóstico durante el desarrollo.

Primero se verificó la comunicación utilizando un registro con valor conocido y posteriormente se comprobó la lectura de las variables reales.

Una vez integrado Rapid SCADA, Communicator pasó a desempeñar el rol de maestro Modbus RTU del sistema.

Modbus Poll no se utiliza simultáneamente como segundo maestro sobre el mismo puerto serie durante la operación normal.

## Consideración para bus RS485 compartido

La dirección Modbus `1` corresponde a la configuración utilizada durante el desarrollo individual del nodo.

En un bus RS485 compartido, cada esclavo debe utilizar una dirección Modbus única.

Por lo tanto, si la cátedra asigna otra dirección al nodo, deben modificarse de manera consistente:

- el Slave ID configurado en el firmware del ESP32;
- la dirección del dispositivo correspondiente en Rapid SCADA.

El mapa HR0-HR6 no necesita modificarse por un cambio de Slave ID.

Asimismo, todos los dispositivos que compartan el mismo bus deben utilizar parámetros de comunicación compatibles.

En una red Modbus RTU convencional sobre RS485 se utiliza un único maestro para coordinar las consultas a los distintos esclavos.

## Estado

La lectura completa de HR0-HR6 mediante Function Code 03 fue verificada tanto con Modbus Poll durante las pruebas como posteriormente con Rapid SCADA Communicator.