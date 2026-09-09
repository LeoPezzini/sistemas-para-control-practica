\# Mapeo de GPIO



\## ESP32 NodeMCU



| Dispositivo | Señal | GPIO | Estado | Observaciones |

|---|---|---:|---|---|

| DHT11 | DATA | 27 | Verificado | Alimentación 3.3 V |

| HC-SR04 | TRIG | 25 | Verificado | Salida directa ESP32 |

| HC-SR04 | ECHO | 26 | Verificado | Divisor 1 kΩ / 2 kΩ |

| MAX485 | DI / TX | 17 | Verificado | UART2 |

| MAX485 | RO / RX | 16 | Verificado | Divisor 1 kΩ / 2 kΩ |

| MAX485 | DE + /RE | 4 | Verificado | Control half-duplex |

| Encoder | Canal A | 32 | Verificado | Pull-up 4.7 kΩ a 3.3 V |

| Encoder | Canal B | 33 | Verificado | Pull-up 4.7 kΩ a 3.3 V |



\## Pines reservados



\- GPIO27: DHT11

\- GPIO25: HC-SR04 TRIG

\- GPIO26: HC-SR04 ECHO

\- GPIO17: RS485 TX

\- GPIO16: RS485 RX

\- GPIO4: RS485 DE/RE

\- GPIO32: Encoder A

\- GPIO33: Encoder B



La fase Z del encoder no se utiliza en la implementación actual.

