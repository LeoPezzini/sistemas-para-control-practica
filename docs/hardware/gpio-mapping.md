\# Mapeo de GPIO



\## ESP32 NodeMCU



| Dispositivo | Señal | GPIO | Estado | Observaciones |

|---|---|---:|---|---|

| DHT11 | DATA | 27 | Verificado | Alimentación 3.3 V |

| HC-SR04 | TRIG | 25 | Verificado | Salida directa ESP32 |

| HC-SR04 | ECHO | 26 | Verificado | Divisor resistivo 1 kΩ / 2 kΩ |

| Encoder | A | - | Pendiente | Esperando especificación de cátedra |

| Encoder | B | - | Pendiente | Esperando especificación de cátedra |

| MAX485 | DI / TX | 17 | Propuesto | UART2 |

| MAX485 | RO / RX | 16 | Propuesto | Requiere adaptación 5 V → 3.3 V |

| MAX485 | DE + RE | 4 | Propuesto | Control de dirección RS485 |

