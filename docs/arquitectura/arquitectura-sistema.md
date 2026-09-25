# Arquitectura del sistema

## Descripción general

El sistema implementado se organiza en tres capas:

1. adquisición y comunicación de campo;
2. supervisión y registro;
3. acceso remoto.

Esta separación distingue la comunicación Modbus RTU utilizada para adquirir las variables del mecanismo utilizado para acceder remotamente a la supervisión.

## 1. Adquisición y comunicación de campo

El nodo principal es un ESP32 NodeMCU conectado a:

- DHT11 para temperatura y humedad relativa;
- HC-SR04 para distancia;
- encoder incremental Omron E6B2-CWZ6C para posición angular.

El ESP32 realiza adquisición y procesamiento y funciona como esclavo Modbus RTU.

```text
DHT11 ───────────────┐
                     │
HC-SR04 ─────────────┼──► ESP32 NodeMCU
                     │      Modbus RTU Slave
Encoder incremental ─┘
                            │
                          UART2
                            │
                          MAX485
                            │
                          RS485
                            │
                       USB-RS485
                            │
                            ▼
                     PC / Rapid SCADA
                       Modbus Master
```

Configuración validada durante el desarrollo:

- 9600 baud;
- 8 bits de datos;
- sin paridad;
- 1 bit de stop;
- Modbus RTU;
- FC03 - Read Holding Registers;
- Slave ID 1 en la configuración individual del nodo.

## 2. Supervisión y registro

Rapid SCADA se ejecuta en la PC. Communicator realiza el polling Modbus y Server almacena y publica los datos.

```text
ESP32 ──RS485/Modbus RTU──► Communicator
                                │
                                ▼
                              Server
                         ┌──────┴──────┐
                         │             │
                      Sec30        Webstation
                  T y HR / 30 s    TCP 10008
```

Los canales 101 y 102, temperatura y humedad, se almacenan en el archivo histórico `Sec30` cada 30 segundos. La exportación histórica a Excel fue verificada.

El ensayo prolongado del 24 al 25 de septiembre de 2026 obtuvo 2793 registros válidos sobre 2878 instantes posibles desde el primer dato válido, equivalente a una disponibilidad de registro de 97.05 %. Las interrupciones detectadas se encuentran documentadas en `docs/pruebas/test-log.md`.

## 3. Acceso remoto

Además del acceso local, Webstation fue accedido desde redes externas mediante Tailscale.

```text
Rapid SCADA → Webstation / TCP 10008 → Tailscale → Internet/VPN → dispositivo autorizado
```

La prueba se realizó tanto desde un teléfono mediante datos móviles como desde otro dispositivo y otra red Wi-Fi. No fue necesario publicar Webstation mediante port forwarding.

### Control de acceso

Para el acceso compartido se aplicó un criterio de mínimo privilegio, restringiendo el acceso al host SCADA y al servicio TCP 10008. No se utilizó el equipo como Exit Node ni como Subnet Router.

No se almacenan en este repositorio credenciales, claves de autenticación ni datos personales utilizados para acceder a la VPN.

## 4. Topología común de la cátedra

La arquitectura de adquisición y acceso remoto del grupo está validada. La eventual topología común para integrar nodos o sistemas de distintos grupos **permanece pendiente de definición por la cátedra**.

Hasta que dicha definición sea confirmada, este documento no presupone:

- qué equipo actuará como maestro del bus común;
- qué Slave ID definitivo utilizará cada grupo;
- qué parámetros físicos o de enlace serán adoptados para el bus compartido;
- si existirá una única instancia SCADA central o una arquitectura distribuida.

Una vez confirmada la topología común, deberán ajustarse únicamente los parámetros que correspondan sin alterar la documentación de las pruebas ya realizadas.

## Independencia entre adquisición y acceso remoto

La VPN no reemplaza ni modifica el enlace RS485.

```text
Adquisición local:
Sensores → ESP32 → Modbus RTU → RS485 → Rapid SCADA

Acceso remoto:
Rapid SCADA → Webstation → TCP/IP → Tailscale → dispositivo remoto
```

Una interrupción de Internet o de la VPN afecta el acceso remoto, pero no implica por sí misma una interrupción de la adquisición local.

## Estado

Adquisición, Modbus RTU, supervisión, registro histórico y acceso remoto mediante VPN: **implementados y verificados**.

Topología común entre grupos: **pendiente de definición por la cátedra**.
