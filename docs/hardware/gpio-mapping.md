\# Mapeo de GPIO



\## ESP32 NodeMCU



| Dispositivo | Señal | GPIO | Estado | Observaciones |

|---|---|---:|---|---|

| DHT11 | DATA | 27 | Verificado | Alimentación 3.3 V |

| HC-SR04 | TRIG | 25 | Verificado | Salida directa |

| HC-SR04 | ECHO | 26 | Verificado | Divisor 1 kΩ / 2 kΩ |

| Encoder | Canal A | - | Pendiente | Esperando especificación |

| Encoder | Canal B | - | Pendiente | Esperando especificación |

| MAX485 | DI / TX | 17 | Verificado | UART2 TX |

| MAX485 | RO / RX | 16 | Verificado | Divisor de nivel 5 V → \~3.3 V |

| MAX485 | DE + /RE | 4 | Verificado | Control dirección RS485 |



\## Pines reservados



GPIO27 queda reservado para DHT11.



GPIO25 y GPIO26 quedan reservados para HC-SR04.



GPIO16, GPIO17 y GPIO4 quedan reservados para comunicación RS485.



La asignación del encoder se realizará una vez conocida su interfaz eléctrica y cantidad de canales.

