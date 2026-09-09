\# Mapa de registros Modbus



\## Configuración



\- Protocolo: Modbus RTU

\- Dispositivo: ESP32 NodeMCU

\- Rol: Slave

\- Slave ID: 1

\- Baud rate: 9600

\- Formato: 8N1

\- Function Code: 03 - Read Holding Registers



\## Holding Registers



| Dirección | Variable | Unidad | Escala | Observaciones |

|---:|---|---|---:|---|

| 0 | Temperatura | °C | ×10 | DHT11 |

| 1 | Humedad relativa | % | ×10 | DHT11 |

| 2 | Distancia sin compensar | cm | ×10 | HC-SR04 |

| 3 | Distancia compensada | cm | ×10 | Compensación por temperatura |

| 4 | Contador encoder HIGH | - | 16 bits | Parte alta del contador de 32 bits |

| 5 | Contador encoder LOW | - | 16 bits | Parte baja del contador de 32 bits |

| 6 | Posición angular | ° | ×10 | Posición relativa 0–360° |



\## Representación del encoder



El contador del encoder se almacena internamente como entero de 32 bits con signo.



Para transmitirlo mediante Modbus se divide en dos registros de 16 bits:



```text

HR4 = palabra alta

HR5 = palabra baja

