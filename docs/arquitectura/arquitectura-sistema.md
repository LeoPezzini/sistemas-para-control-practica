\# Arquitectura del sistema



\## Estado



\*\*Arquitectura preliminar - pendiente de validación con la cátedra.\*\*



\## Arquitectura actualmente evaluada



La arquitectura propuesta utiliza un ESP32 NodeMCU como nodo de adquisición.



```text

DHT11 ───────┐

&#x20;            │

HC-SR04 ─────┼──► ESP32 NodeMCU

&#x20;            │

Encoder ─────┘

&#x20;                 │

&#x20;                 │ UART2

&#x20;                 ▼

&#x20;               MAX485

&#x20;                 │

&#x20;               RS485

&#x20;                 │

&#x20;            USB-RS485

&#x20;                 │

&#x20;                 ▼

&#x20;                PC

&#x20;                 │

&#x20;            RapidSCADA

&#x20;         Modbus RTU Master

