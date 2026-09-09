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

Pendiente de ensayo.

### Estado

**PENDIENTE**

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
