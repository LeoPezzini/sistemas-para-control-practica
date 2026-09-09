\# Registro de decisiones de diseño



| ID | Fecha | Decisión | Justificación | Estado |

|---|---|---|---|---|

| DEC-001 | 2026-08-31 | Evaluar la Waveshare ESP32-S3-POE-ETH-8DI-8DO como plataforma principal | Plataforma provista por la cátedra | En revisión |

| DEC-002 | 2026-08-31 | Utilizar RS485 como medio de comunicación | Requisito definido para la práctica | Aprobada |

| DEC-003 | 2026-09-01 | Utilizar GPIO27 para DHT11 en ESP32 NodeMCU | Funcionamiento verificado experimentalmente | Aprobada |

| DEC-004 | 2026-09-01 | Utilizar dhtESP32-rmt para adquisición DHT11 | Funcionamiento estable verificado sobre ESP32 | Aprobada |

| DEC-005 | 2026-09-01 | Evaluar ESP32 NodeMCU como nodo principal de adquisición | DHT11 y HC-SR04 fueron verificados correctamente sobre GPIO directos del ESP32 | En evaluación |

| DEC-006 | 2026-09-02 | Utilizar MAX485 disponible para pruebas RS485 | Permite avanzar sin costo adicional; se incorpora adaptación de nivel en RO | Aprobada |

| DEC-007 | 2026-09-02 | Utilizar GPIO17, GPIO16 y GPIO4 para RS485 | Configuración verificada mediante comunicación ESP32-PC | Aprobada |

| DEC-008 | 2026-09-02 | Utilizar ESP32 como Modbus RTU Slave ID 1 | La comunicación bidireccional fue verificada mediante Modbus Poll sobre RS485 | Aprobada |

| DEC-009 | 2026-09-04 | Compensar la medición ultrasónica utilizando la temperatura medida por el DHT11 | La velocidad del sonido depende de la temperatura y la compensación constituye un requisito explícito de la práctica | Aprobada |

| DEC-010 | 2026-09-04 | Conservar en Modbus la distancia sin compensar y la distancia compensada | Permite evaluar posteriormente el efecto de la compensación durante la caracterización contra el patrón | Aprobada |

| DEC-011 | 2026-09-09 | Utilizar GPIO32 y GPIO33 para las fases A y B del encoder | Pines disponibles y funcionamiento verificado experimentalmente | Aprobada |

| DEC-012 | 2026-09-09 | Representar la posición angular del encoder entre 0° y 360° | El enunciado requiere adquirir posición angular de un encoder incremental | Aprobada |

| DEC-013 | 2026-09-09 | Utilizar 2000 cuentas por revolución en la implementación actual | Se detectan ambos flancos de la fase A en un encoder de 1000 P/R | Aprobada |

| DEC-014 | 2026-09-09 | Transmitir el contador del encoder en dos registros Modbus de 16 bits | Permite conservar el contador de 32 bits con signo sin limitarlo a un único Holding Register | Aprobada |

