\# Arquitectura del sistema



\## Descripción general



El sistema implementado se organiza en tres capas principales:



1\. adquisición y comunicación de campo;

2\. supervisión y registro;

3\. acceso remoto.



Esta separación permite distinguir la comunicación Modbus RTU utilizada para la adquisición de variables del mecanismo utilizado para acceder remotamente a la interfaz de supervisión.



\## 1. Adquisición y comunicación de campo



El nodo principal de adquisición es un ESP32 NodeMCU.



Al ESP32 se conectan:



\- un DHT11 para temperatura y humedad relativa;

\- un HC-SR04 para medición de distancia;

\- un encoder incremental Omron E6B2-CWZ6C para posición angular.



El ESP32 realiza la adquisición y procesamiento de las variables y funciona como esclavo Modbus RTU.



La comunicación con la PC se realiza mediante UART2, un transceptor MAX485 y un conversor USB-RS485.



```text

DHT11 ───────────────┐

&#x20;                    │

HC-SR04 ─────────────┼──► ESP32 NodeMCU

&#x20;                    │      Modbus RTU Slave

Encoder incremental ─┘

&#x20;                           │

&#x20;                         UART2

&#x20;                           │

&#x20;                           ▼

&#x20;                        MAX485

&#x20;                           │

&#x20;                         RS485

&#x20;                           │

&#x20;                           ▼

&#x20;                       USB-RS485

&#x20;                           │

&#x20;                           ▼

&#x20;                    PC / Rapid SCADA

&#x20;                     Modbus Master

```



La configuración utilizada para Modbus RTU es:



\- 9600 baud;

\- 8 bits de datos;

\- sin paridad;

\- 1 bit de stop;

\- modo RTU;

\- Function Code 03 para lectura de Holding Registers.



\## 2. Supervisión y registro



Rapid SCADA se ejecuta en la PC y actúa como maestro Modbus RTU.



Communicator realiza el polling del ESP32 y entrega los datos adquiridos al resto del sistema SCADA.



La arquitectura de supervisión implementada es:



```text

&#x20;                   Rapid SCADA

&#x20;                        │

&#x20;             ┌──────────┴──────────┐

&#x20;             │                     │

&#x20;         Communicator            Server

&#x20;             │                     │

&#x20;        Modbus RTU          Archivo histórico

&#x20;                                 Sec30

&#x20;                                   │

&#x20;                        Temperatura y humedad

&#x20;                             cada 30 segundos



&#x20;                        Rapid SCADA

&#x20;                             │

&#x20;                             ▼

&#x20;                         Webstation

&#x20;                         TCP 10008

&#x20;                             │

&#x20;                             ▼

&#x20;                        Navegador web

```



Los canales de temperatura y humedad se almacenan en el archivo histórico `Sec30` con un período de escritura de 30 segundos.



Webstation permite visualizar mediante navegador las variables adquiridas y consultar la información registrada.



\## 3. Acceso remoto



Además del acceso desde la red local, se implementó acceso remoto a Webstation a través de Internet mediante una VPN basada en Tailscale.



La VPN se incorpora únicamente en la capa de acceso a la supervisión y no modifica la arquitectura de adquisición ni la comunicación Modbus RTU.



```text

Sensores

&#x20;  │

&#x20;  ▼

ESP32

&#x20;  │

Modbus RTU / RS485

&#x20;  │

&#x20;  ▼

Rapid SCADA

&#x20;  │

&#x20;  ▼

Webstation

TCP 10008

&#x20;  │

&#x20;  ▼

Tailscale

&#x20;  │

Internet / VPN

&#x20;  │

&#x20;  ▼

Dispositivo autorizado

```



El acceso fue verificado utilizando un teléfono conectado mediante datos móviles, sin utilizar la red Wi-Fi local.



De esta forma se comprobó el acceso a Webstation desde una red externa a través de la VPN.



\## Control de acceso



La configuración de Tailscale se realizó aplicando un criterio de mínimo privilegio.



Para los dispositivos compartidos se restringe el acceso al equipo que ejecuta Rapid SCADA y específicamente al servicio necesario para Webstation:



```text

Usuario/dispositivo compartido

&#x20;           │

&#x20;           ▼

&#x20;      Host SCADA

&#x20;           │

&#x20;           ▼

&#x20;      TCP 10008

&#x20;           │

&#x20;           ▼

&#x20;      Webstation

```



No se utiliza el equipo SCADA como exit node ni como router de subred para proporcionar acceso general a la red local.



No se almacenan en este repositorio credenciales, claves de autenticación ni información personal utilizada para acceder a la VPN.



\## Acceso del supervisor



La arquitectura permite compartir con el supervisor el equipo que ejecuta Rapid SCADA sin requerir que todos los grupos formen parte de una única infraestructura de adquisición.



Cada grupo puede mantener su propio sistema de adquisición y compartir únicamente el acceso necesario a su Webstation.



Conceptualmente:



```text

Grupo A ──► VPN ──┐

&#x20;                 │

Grupo B ──► VPN ──┼──► Supervisor

&#x20;                 │

Grupo C ──► VPN ──┘

```



De esta manera se centraliza el acceso del supervisor sin centralizar la infraestructura de adquisición de los distintos grupos.



\## Independencia entre adquisición y acceso remoto



La incorporación de la VPN no reemplaza ni modifica el enlace RS485.



La cadena de adquisición permanece:



```text

Sensores → ESP32 → Modbus RTU → RS485 → Rapid SCADA

```



mientras que el acceso remoto se realiza posteriormente:



```text

Rapid SCADA → Webstation → TCP/IP → Tailscale → dispositivo remoto

```



Por lo tanto, una interrupción de la conexión a Internet o de la VPN afecta el acceso remoto, pero no constituye por sí misma una interrupción de la adquisición local mediante Modbus RTU ni del procesamiento realizado por Rapid SCADA.



\## Estado



La arquitectura de adquisición, comunicación Modbus RTU, supervisión mediante Rapid SCADA, registro histórico y acceso remoto mediante VPN fue implementada y verificada experimentalmente.