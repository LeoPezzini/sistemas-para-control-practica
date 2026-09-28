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

---

## TEST-005 - Comunicación Modbus RTU mediante Modbus Poll

**Fecha:** 2026-09-02

### Objetivo

Verificar la implementación del protocolo Modbus RTU entre el ESP32 NodeMCU y la PC utilizando la interfaz RS485 previamente validada.

### Arquitectura de prueba


```text
ESP32 NodeMCU
→ UART2
→ MAX485
→ RS485
→ conversor USB-RS485
→ PC
→ Modbus Poll
```
### Configuración

El ESP32 NodeMCU se configuró como esclavo Modbus RTU con los siguientes parámetros:

- Slave ID: 1;
- baud rate: 9600 bit/s;
- bits de datos: 8;
- paridad: ninguna;
- bits de stop: 1;
- modo: RTU.

La PC se utilizó como maestro Modbus mediante Modbus Poll.

Para la primera verificación se implementó un Holding Register de prueba con valor fijo:

`1234`

La lectura se realizó mediante Function Code 03 - Read Holding Registers.

### Procedimiento

Una vez verificada previamente la comunicación física RS485, se cargó en el ESP32 un firmware de prueba utilizando la librería `ModbusRTU`.

Desde Modbus Poll se configuró una consulta al Slave ID 1 utilizando Function Code 03 y se solicitó la lectura del Holding Register correspondiente.

### Resultado

Modbus Poll recibió correctamente el valor:

`1234`

La recepción correcta del registro permitió verificar:

- comunicación física RS485;
- configuración coincidente de velocidad y formato serie;
- direccionamiento del esclavo Modbus;
- funcionamiento de la consulta mediante Function Code 03;
- respuesta del ESP32 como esclavo Modbus RTU.

A partir de esta prueba se avanzó posteriormente a reemplazar el valor fijo por las variables adquiridas mediante los sensores.

### Estado

**APROBADO**

---

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

La verificación puntual del período de 30 s se complementó posteriormente con el ensayo prolongado documentado en TEST-016.

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

---

## TEST-014 - Acceso remoto a Webstation mediante Tailscale

**Fecha:** 2026-09-19

### Objetivo

Verificar el acceso a Rapid SCADA Webstation a través de Internet utilizando una red privada virtual basada en Tailscale, evitando la exposición directa del servicio mediante port forwarding.

### Configuración

- servidor: PC con Rapid SCADA Webstation;
- puerto de Webstation: TCP 10008;
- VPN: Tailscale;
- acceso mediante la dirección asignada por la red Tailscale;
- sin utilización de Exit Node;
- sin utilización de Subnet Router;
- acceso del usuario compartido restringido al host SCADA y al servicio TCP 10008.

### Procedimiento

Se realizaron dos verificaciones de acceso remoto.

En una primera prueba se accedió a Webstation desde un teléfono móvil conectado mediante datos móviles, sin utilizar la red Wi-Fi local de la PC SCADA.

Posteriormente se compartió la PC SCADA mediante Tailscale con otro integrante del grupo. Este realizó una segunda prueba desde su teléfono conectado a una red Wi-Fi externa, correspondiente a otra ubicación física y otra conexión a Internet.

En ambos casos se accedió a Webstation utilizando la dirección Tailscale de la PC y el puerto TCP 10008.

### Resultado

Rapid SCADA Webstation cargó correctamente desde ambas redes externas.

La segunda prueba permitió verificar además el acceso desde:

- otro usuario;
- otro dispositivo;
- otra red local;
- otra conexión a Internet.

De esta forma se verificó el acceso remoto al sistema SCADA sin necesidad de exponer directamente Webstation a Internet mediante configuración de port forwarding en el router.

### Consideraciones de seguridad

Se adoptó un criterio de mínimo privilegio.

La configuración de acceso compartido permite alcanzar el host SCADA mediante TCP 10008, correspondiente a Webstation, sin utilizar la PC como Exit Node ni como router de subred.

La prueba de acceso remoto mediante VPN corresponde al SCADA propio del grupo. Posteriormente, la cátedra definió por separado una topología de integración común basada en un único maestro Modbus RTU sobre RS485.

### Estado

**APROBADO**

---

## TEST-015 - Caracterización del HC-SR04 contra UNI-T LM50A

**Fecha:** 2026-09-18

### Objetivo

Caracterizar experimentalmente la medición de distancia realizada por el HC-SR04 utilizando el medidor UNI-T LM50A como instrumento de referencia y evaluar el efecto de la compensación por temperatura implementada en el firmware.

### Referencia utilizada

Se utilizó el medidor UNI-T LM50A como referencia para las distancias del ensayo.

La documentación consultada para el HC-SR04 indica:

- rango de medición: 2 a 400 cm;
- resolución: 0.3 cm.

La resolución indicada no se interpreta como exactitud ni como error máximo del sensor.

### Datos experimentales

| UNI-T LM50A [cm] | HC-SR04 sin compensar [cm] | HC-SR04 compensado [cm] | Error sin compensar [cm] | Error compensado [cm] |
|---:|---:|---:|---:|---:|
| 27 | 26.5 | 26.8 | -0.5 | -0.2 |
| 34 | 33.4 | 33.8 | -0.6 | -0.2 |
| 44 | 41.9 | 42.4 | -2.1 | -1.6 |
| 39 | 37.4 | 37.9 | -1.6 | -1.1 |
| 30 | 28.8 | 29.2 | -1.2 | -0.8 |
| 21 | 19.4 | 19.6 | -1.6 | -1.4 |

El error se calculó como:

`Error = distancia HC-SR04 - distancia de referencia`

### Resultados

A partir de los seis puntos medidos se obtuvo:

- error absoluto medio (MAE) sin compensación: aproximadamente 1.27 cm;
- error absoluto medio (MAE) con compensación: aproximadamente 0.88 cm;
- error absoluto máximo sin compensación: 2.1 cm;
- error absoluto máximo con compensación: 1.6 cm.

La compensación por temperatura redujo el error absoluto medio aproximadamente un 30 % respecto del cálculo realizado utilizando una velocidad fija del sonido.

En los seis puntos ensayados el error resultó negativo, observándose una tendencia del HC-SR04 a subestimar la distancia respecto del instrumento de referencia bajo las condiciones particulares del ensayo.

Asimismo, la compensación por temperatura produjo una reducción del error absoluto en todos los puntos registrados.

### Limitaciones del ensayo

La caracterización se realizó con los medios disponibles en el lugar de trabajo.

No fue posible comenzar la serie de mediciones a 10 cm debido a las condiciones de utilización del instrumento de referencia.

Además, durante el ensayo fue necesario utilizar dos superficies de apoyo que no presentaban exactamente la misma altura y nivelación. Por este motivo pudieron existir variaciones de alineación entre el sensor, el instrumento de referencia y el objeto utilizado como blanco.

Estas condiciones se consideran posibles fuentes de incertidumbre experimental y deben tenerse en cuenta al interpretar los resultados.

### Conclusión

Los resultados obtenidos muestran que la compensación de la velocidad del sonido mediante la temperatura medida por el DHT11 mejoró la concordancia con el instrumento de referencia en todos los puntos ensayados.

No se aplicó una corrección empírica adicional al firmware a partir de estos resultados.

### Estado

**APROBADO**

---

## TEST-016 - Registro continuo de temperatura y humedad durante 24 horas

**Fecha:** 2026-09-24 a 2026-09-25

### Objetivo

Verificar el funcionamiento prolongado del sistema de adquisición y almacenamiento histórico, registrando temperatura y humedad relativa con un período de 30 segundos.

### Configuración

- variables: temperatura y humedad relativa;
- canales Rapid SCADA: 101 y 102;
- archivo histórico: `Sec30`;
- período de almacenamiento: 30 s;
- cadena: ESP32 → Modbus RTU → RS485 → Rapid SCADA Communicator → Server → `Sec30`.

### Resultado

La exportación de Rapid SCADA comprendió la ventana 24/09/2026 15:30:00 a 25/09/2026 15:40:00. Los primeros registros válidos aparecen a las 15:41:30 del 24/09.

Considerando desde el primer dato válido hasta el final:

- instantes de muestreo posibles: 2878;
- registros válidos simultáneos de temperatura y humedad: 2793;
- registros faltantes: 85;
- disponibilidad de registro: **97.05 %**.

Se identificaron tres intervalos sin datos:

| Intervalo | Muestras faltantes | Duración representada |
|---|---:|---:|
| 24/09 16:43:30 - 16:44:30 | 3 | 1.5 min |
| 25/09 09:06:00 - 09:12:00 | 13 | 6.5 min |
| 25/09 09:16:30 - 09:50:30 | 69 | 34.5 min |

La interrupción de mayor duración coincidió temporalmente con trabajos realizados sobre otro montaje en la misma mesa, con movimientos y vibraciones próximos al prototipo. Al detectarse la ausencia de nuevas mediciones se revisó físicamente el montaje y la adquisición se restableció. No fue posible determinar de manera concluyente la causa, por lo que los intervalos se documentan como datos faltantes y no se interpolan.

### Variables registradas

- temperatura: mínimo 26 °C, máximo 30 °C, promedio 28.32 °C;
- humedad relativa: mínimo 5 %, máximo 6 %, promedio 5.50 %.

Los valores bajos de humedad coincidieron con condiciones ambientales extremadamente secas durante un episodio de viento Zonda. Como comprobación funcional posterior, el DHT11 respondió ante un incremento local de humedad, elevando su lectura aproximadamente hasta 75 % HR. Esta prueba verifica que el canal responde a cambios de humedad, pero no constituye una calibración ni una verificación de exactitud.

### Conclusión

El sistema completó el ensayo prolongado y mantuvo el registro periódico durante la mayor parte de la ventana evaluada. Las interrupciones observadas quedan documentadas como parte del resultado experimental y permiten cuantificar una disponibilidad de registro del 97.05 %.

### Estado

**APROBADO CON INTERRUPCIONES DOCUMENTADAS**


---

## TEST-017 - Integración en bus RS485 común con maestro Modbus

**Fecha:** 2026-09-25

### Objetivo

Verificar que el nodo ESP32 del grupo pueda integrarse junto con los nodos de otros grupos en un bus RS485 compartido y ser consultado por un único maestro Modbus RTU.

### Configuración

- medio físico: RS485;
- protocolo: Modbus RTU;
- función de lectura del nodo: FC03 - Read Holding Registers;
- mapa del nodo: HR0-HR6;
- Slave ID utilizado durante la integración: **3**;
- maestro: sistema común dispuesto por la cátedra.

El template Modbus del nodo se utilizó para describir al maestro las variables disponibles. El mapa de registros no requirió modificaciones respecto de las pruebas individuales.

### Procedimiento

Se modificó el Slave ID del ESP32 desde el valor 1 utilizado durante el desarrollo individual al valor 3 asignado para la integración. El firmware actualizado fue cargado en el ESP32 y el nodo se conectó al bus RS485 común.

### Resultado

El maestro Modbus pudo consultar correctamente el nodo del grupo junto con los demás nodos conectados al bus.

La integración confirmó que el direccionamiento mediante Slave ID permite compartir el mismo medio RS485 manteniendo el mapa de registros propio de cada dispositivo.

### Estado

**APROBADO**
