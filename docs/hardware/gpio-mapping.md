\# Mapeo de GPIO



\## ESP32 NodeMCU



| Dispositivo | Señal | GPIO | Estado | Observaciones |

|---|---|---:|---|---|

| DHT11 | DATA | 27 | Verificado | Alimentación 3.3 V |

| HC-SR04 | TRIG | 25 | Verificado | Salida directa ESP32 |

| HC-SR04 | ECHO | 26 | Verificado | Divisor 1 kΩ / 2 kΩ |

| MAX485 | DI / TX | 17 | Verificado | UART2 TX |

| MAX485 | RO / RX | 16 | Verificado | UART2 RX mediante divisor 1 kΩ / 2 kΩ |

| MAX485 | DE + /RE | 4 | Verificado | Control de dirección half-duplex |

| Encoder | Canal A | 32 | Verificado | Pull-up externo 4.7 kΩ a 3.3 V |

| Encoder | Canal B | 33 | Verificado | Pull-up externo 4.7 kΩ a 3.3 V |



\---



\## Alimentación y adaptación de niveles



\### DHT11



\- VCC: 3.3 V

\- GND: común

\- DATA: GPIO27



\### HC-SR04



\- VCC: 5 V

\- GND: común

\- TRIG: GPIO25

\- ECHO: GPIO26 mediante divisor resistivo



Divisor ECHO:



```text

HC-SR04 ECHO

&#x20;    │

&#x20;   1 kΩ

&#x20;    │

&#x20;    ├──── GPIO26

&#x20;    │

&#x20;   2 kΩ

&#x20;    │

&#x20;   GND

