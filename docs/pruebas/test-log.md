# Registro de pruebas

## TEST-001 - Programación ESP32 NodeMCU

**Fecha:** 2026-09-01

### Objetivo

Verificar comunicación USB, programación y ejecución de firmware sobre ESP32 NodeMCU.

### Procedimiento

Se instaló el driver CP2102 y se seleccionó el puerto COM correspondiente.

Se cargó un programa Blink utilizando GPIO2.

### Resultado

El firmware fue cargado correctamente y el LED respondió según lo esperado.

### Estado

**APROBADO**

---

## TEST-002 - DHT11 sobre ESP32 NodeMCU

**Fecha:** 2026-09-01

### Objetivo

Verificar adquisición de temperatura y humedad mediante DHT11.

### Configuración

- VCC: 3.3 V
- GND: GND
- DATA: GPIO27
- Librería: dhtESP32-rmt

### Resultado

Se obtuvieron lecturas estables.

Valores preliminares:

- Temperatura: aproximadamente 23 °C
- Humedad: aproximadamente 18 %

Inicialmente se observaron errores de comunicación debido a un falso contacto en la alimentación del sensor.

Una vez corregida la conexión, la adquisición resultó estable.

### Estado

**APROBADO**

---

## TEST-003 - HC-SR04 sobre ESP32 NodeMCU

**Fecha:** 2026-09-01

### Objetivo

Verificar medición de distancia mediante sensor ultrasónico HC-SR04.

### Configuración

- VCC: 5 V
- GND: GND
- TRIG: GPIO25
- ECHO: GPIO26

La señal ECHO se adaptó mediante divisor resistivo:

- R superior: 1 kΩ
- R inferior: 2 kΩ

### Resultado

El sensor respondió correctamente.

La medición fue comparada preliminarmente con una cinta métrica.

Se observó una diferencia aproximada de 1 cm.

### Observaciones

La diferencia observada no será corregida empíricamente en esta etapa.

La calibración definitiva se realizará utilizando el patrón de referencia provisto por la cátedra y considerando posteriormente la compensación ambiental correspondiente.

### Estado

**APROBADO FUNCIONALMENTE**

---

## TEST-004 - Comunicación RS485

**Fecha:** 2026-09-02

### Objetivo

Verificar la capa física de comunicación entre ESP32 y PC mediante RS485.

### Arquitectura de prueba

ESP32 NodeMCU
→ UART2
→ MAX485
→ RS485
→ conversor USB-RS485
→ PC

### Configuración ESP32

- TX2: GPIO17
- RX2: GPIO16
- DE + /RE: GPIO4
- Baud rate: 9600
- Formato: 8N1

### Adaptación de nivel

El MAX485 se alimentó a 5 V.

La salida RO fue conectada a GPIO16 mediante divisor resistivo 1 kΩ / 2 kΩ.

Se midieron aproximadamente 3.1 V en la entrada GPIO16.

### Procedimiento

El ESP32 transmitió periódicamente:

`HOLA RS485 n`

Los datos fueron recibidos mediante PuTTY utilizando el puerto COM correspondiente al adaptador USB-RS485.

### Resultado

La recepción de mensajes fue correcta y continua.

### Estado

**APROBADO**

### Próximo ensayo

Implementación de Modbus RTU entre ESP32 y Modbus Poll.

## TEST-006 - Integración DHT11 + HC-SR04 + Modbus RTU

**Fecha:** 2026-09-04

### Objetivo

Verificar la adquisición simultánea de temperatura, humedad y distancia y su publicación mediante registros Modbus RTU.

### Configuración

ESP32 NodeMCU configurado como Modbus RTU Slave ID 1.

Mapa de registros:

| Registro | Variable | Escala |
|---|---|---|
| HR0 | Temperatura | valor × 10 |
| HR1 | Humedad relativa | valor × 10 |
| HR2 | Distancia | valor × 10 |

### Resultado

Modbus Poll leyó correctamente los tres Holding Registers mediante Function Code 03.

Durante la prueba se observaron, entre otros, los siguientes valores:

- HR0 = 240 → 24.0 °C
- HR1 = 90 → 9.0 % HR
- HR2 = 121 → 12.1 cm

Se modificó físicamente la distancia entre el HC-SR04 y el objeto de prueba, observándose la correspondiente variación del registro HR2 en Modbus Poll.

No se observaron errores de comunicación Modbus durante la prueba (`Err = 0`).

### Estado

**APROBADO**

## TEST-007 - Compensación térmica HC-SR04

**Fecha:** 2026-09-04

### Objetivo

Verificar la incorporación de la temperatura medida por el DHT11
al cálculo de distancia del HC-SR04.

### Método

Se calculan simultáneamente:

- distancia utilizando velocidad fija del sonido de 343 m/s;
- distancia utilizando velocidad compensada:

`c(T) = 331.3 + 0.606*T`

Los resultados se publican mediante:

- HR2: distancia sin compensar ×10;
- HR3: distancia compensada ×10.

### Resultado

Durante el ensayo se obtuvieron:

- temperatura: 24.0 °C;
- velocidad compensada: 345.84 m/s;
- tiempo de eco: 747 µs;
- distancia sin compensar: 12.81 cm;
- distancia compensada: 12.92 cm.

Mediante Modbus Poll se observaron:

- HR0 = 240;
- HR1 = 80;
- HR2 = 128;
- HR3 = 129.

La comunicación Modbus se mantuvo sin errores durante el ensayo.

### Estado

**APROBADO**

### Observación

La verificación funcional de la compensación no reemplaza la
caracterización definitiva del HC-SR04 contra el instrumento patrón.

## TEST-008 - Encoder incremental Omron E6B2-CWZ6C

**Fecha:** 2026-09-09

### Objetivo

Verificar el funcionamiento del encoder incremental provisto por la cátedra antes de integrarlo al sistema.

### Modelo

- Omron E6B2-CWZ6C
- Resolución: 1000 P/R
- Salida: NPN open collector
- Señales utilizadas: A y B

### Configuración

- alimentación: 5 V;
- OUT A → GPIO32 con pull-up externo de 4.7 kΩ a 3.3 V;
- OUT B → GPIO33 con pull-up externo de 4.7 kΩ a 3.3 V;
- GND común con ESP32;
- fase Z no utilizada.

### Resultado

El contador respondió correctamente al giro del eje:

- sentido antihorario: incremento;
- sentido horario: decremento;
- eje detenido: contador estable.

La detección del sentido mediante las fases A y B resultó correcta.

### Estado

**APROBADO**

## TEST-009 - Integración encoder + Modbus RTU

**Fecha:** 2026-09-09

### Objetivo

Verificar la adquisición de posición angular del encoder y su publicación mediante Modbus RTU.

### Configuración

Se utilizó el firmware integrado con:

- DHT11;
- HC-SR04;
- compensación por temperatura;
- encoder incremental;
- Modbus RTU Slave ID 1.

### Mapa de registros asociados al encoder

- HR4: contador encoder - palabra alta;
- HR5: contador encoder - palabra baja;
- HR6: posición angular ×10.

### Método

Se marcó físicamente una posición inicial del eje.

Se reinició el sistema tomando dicha posición como 0°.

Luego se realizó una revolución completa hasta regresar a la marca inicial.

### Resultado

La posición angular publicada en HR6 volvió a aproximadamente 0° al regresar a la posición inicial.

También se verificó que:

- el ángulo aumenta en sentido antihorario;
- el ángulo disminuye en sentido horario;
- el valor se mantiene estable con el eje detenido;
- la comunicación Modbus se mantuvo sin errores.

### Estado

**APROBADO**

### TEST-010 - Comunicación Rapid SCADA ↔ ESP32 mediante Modbus RTU

**Objetivo:** verificar la adquisición de los registros del nodo ESP32
desde Rapid SCADA actuando como maestro Modbus RTU.

**Configuración:**
- Maestro: Rapid SCADA Communicator
- Puerto: COM8
- Medio físico: USB-RS485 + MAX485
- Protocolo: Modbus RTU
- Baud rate: 9600 bit/s
- Formato: 8N1
- Esclavo: ID 1
- Función: 03 - Read Holding Registers
- Dirección inicial: 0
- Cantidad: 7 registros

**Resultado:**
- Communication channel: COM8, open
- ESP32 NodeMCU: Normal
- Petición observada: 01 03 00 00 00 07 ...
- Respuestas recibidas correctamente.
- HR0...HR6 adquiridos sin errores.
- Estado del polling: OK.

**Estado:** APROBADO

---

## TEST-011 - Supervisión mediante Rapid SCADA Webstation

**Fecha:** 2026-09-10

### Objetivo

Verificar la visualización de las variables adquiridas por el ESP32
mediante Rapid SCADA Webstation.

### Configuración

Se utilizó la vista:

`ESP32.tbl`

Canales visualizados:

- 101 - Temperatura;
- 102 - Humedad;
- 103 - Distancia sin compensación;
- 104 - Distancia compensada;
- 105 - Encoder HIGH;
- 106 - Encoder LOW;
- 107 - Posición angular.

### Procedimiento

Se modificó físicamente la distancia medida por el HC-SR04 y se giró
el eje del encoder mientras Webstation permanecía abierto.

### Resultado

Las variables mostradas en Webstation respondieron dinámicamente
a los cambios realizados sobre los sensores.

Se verificó la cadena completa:

ESP32 → Modbus RTU → RS485 → Communicator → Server → Webstation.

### Estado

**APROBADO**

---

## TEST-012 - Registro histórico de temperatura y humedad cada 30 segundos

**Fecha:** 2026-09-10

### Objetivo

Verificar el almacenamiento histórico de temperatura y humedad con
un período de 30 segundos y su posterior exportación a Microsoft Excel.

### Configuración

Archivo histórico:

- Code: `Sec30`;
- Name: `30 Second Archive`;
- Kind: `Historical`;
- Module: `ModArcBasic`;
- Writing period: 30 s;
- Writing offset: 0 s.

Canales asociados:

- 101 - Temperatura;
- 102 - Humedad.

En la base de configuración el archivo personalizado se encuentra
asociado al bit 16.

### Procedimiento

Se dejó el sistema adquiriendo datos y posteriormente se generó
un Historical Data Report utilizando el archivo `Sec30`.

El reporte fue exportado a Microsoft Excel.

### Resultado

Se observaron muestras consecutivas con timestamps:

- 11:10:00
- 11:10:30
- 11:11:00
- 11:11:30
- 11:12:00
- 11:12:30

Esto confirma experimentalmente que el período de almacenamiento
del archivo histórico es de 30 segundos.

La exportación del reporte a Microsoft Excel se realizó correctamente.

### Estado

**APROBADO**

### Observación

Queda pendiente realizar la adquisición definitiva durante 24 horas.

---

## TEST-013 - Acceso remoto a Webstation desde dispositivo móvil

**Fecha:** 2026-09-10

### Objetivo

Verificar el acceso a Rapid SCADA Webstation desde un dispositivo
diferente de la PC que ejecuta el servidor.

### Configuración

- Webstation: TCP puerto 10008;
- PC y teléfono conectados a la misma red local;
- acceso mediante dirección IPv4 local de la PC;
- regla de entrada de Windows Firewall habilitada para TCP 10008.

### Procedimiento

Inicialmente se intentó acceder desde el teléfono con Windows Firewall
activo, sin obtener conexión.

Se desactivó temporalmente el firewall como prueba diagnóstica,
verificándose que el teléfono podía acceder correctamente a Webstation.

Posteriormente se habilitó una regla de entrada específica para
TCP 10008 y se volvió a activar Windows Firewall.

### Resultado

El teléfono pudo acceder correctamente a Webstation con Windows
Firewall activo.

Se verificó el acceso a la interfaz SCADA desde un navegador de
otro dispositivo conectado a la misma red local.

### Estado

**APROBADO**
