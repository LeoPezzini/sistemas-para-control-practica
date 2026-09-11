\# Registro de decisiones de diseño



| ID | Fecha | Decisión | Justificación | Estado |

|---|---|---|---|---|

| DEC-001 | 2026-08-31 | Evaluar la Waveshare ESP32-S3-POE-ETH-8DI-8DO como plataforma principal | Plataforma provista inicialmente por la cátedra | Cerrada |

| DEC-002 | 2026-08-31 | Utilizar RS485 como medio de comunicación | Requisito definido para la práctica | Aprobada |

| DEC-003 | 2026-09-01 | Utilizar GPIO27 para DHT11 en ESP32 NodeMCU | Funcionamiento verificado experimentalmente | Aprobada |

| DEC-004 | 2026-09-01 | Utilizar `dhtESP32-rmt` para adquisición DHT11 | Funcionamiento estable verificado sobre ESP32 | Aprobada |

| DEC-005 | 2026-09-01 | Utilizar ESP32 NodeMCU como nodo principal de adquisición | DHT11, HC-SR04 y comunicación fueron verificados sobre GPIO directos. La utilización del NodeMCU fue posteriormente aceptada por la cátedra | Aprobada |

| DEC-006 | 2026-09-02 | Utilizar MAX485 disponible para RS485 | Permite implementar la interfaz física requerida; se incorpora adaptación de nivel en RO | Aprobada |

| DEC-007 | 2026-09-02 | Utilizar GPIO17, GPIO16 y GPIO4 para RS485 | Configuración verificada mediante comunicación ESP32-PC | Aprobada |

| DEC-008 | 2026-09-02 | Utilizar ESP32 como Modbus RTU Slave ID 1 | La comunicación fue verificada mediante Modbus Poll sobre RS485 | Aprobada |

| DEC-009 | 2026-09-04 | Compensar la medición ultrasónica utilizando la temperatura medida por el DHT11 | La velocidad del sonido depende de la temperatura y la compensación constituye un requisito explícito de la práctica | Aprobada |

| DEC-010 | 2026-09-04 | Conservar en Modbus la distancia sin compensar y la distancia compensada | Permite evaluar posteriormente el efecto de la compensación durante la caracterización contra el patrón | Aprobada |

| DEC-011 | 2026-09-09 | Utilizar GPIO32 y GPIO33 para las fases A y B del encoder | Pines disponibles y funcionamiento verificado experimentalmente | Aprobada |

| DEC-012 | 2026-09-09 | Representar la posición angular del encoder entre 0° y 360° | El enunciado requiere adquirir posición angular de un encoder incremental | Aprobada |

| DEC-013 | 2026-09-09 | Utilizar 2000 cuentas por revolución en la implementación actual | Se detectan ambos flancos de la fase A en un encoder de 1000 P/R | Aprobada |

| DEC-014 | 2026-09-09 | Transmitir el contador del encoder en dos registros Modbus de 16 bits | Permite conservar el contador de 32 bits con signo sin limitarlo a un único Holding Register | Aprobada |

| DEC-015 | 2026-09-10 | Configurar Rapid SCADA Communicator como maestro Modbus RTU sobre COM8 | Permite integrar el nodo ESP32 al sistema SCADA utilizando la misma configuración 9600 8N1 previamente validada | Aprobada |

| DEC-016 | 2026-09-10 | Leer HR0-HR6 como un único bloque contiguo mediante FC03 | Reduce la configuración y permite adquirir las siete variables mediante una única petición Modbus | Aprobada |

| DEC-017 | 2026-09-10 | Utilizar canales 101-107 para representar las variables del ESP32 en Rapid SCADA | Permite asociar los registros Modbus con variables identificables dentro del sistema SCADA | Aprobada |

| DEC-018 | 2026-09-10 | Aplicar `Cnl \* 0.1` en los canales transmitidos con escala ×10 | El ESP32 transmite estas magnitudes como enteros para transportarlas mediante registros Modbus de 16 bits | Aprobada |

| DEC-019 | 2026-09-10 | Crear el archivo histórico personalizado `Sec30` con período de escritura de 30 s | Permite cumplir el requisito de registrar temperatura y humedad cada 30 segundos | Aprobada |

| DEC-020 | 2026-09-10 | Asociar únicamente temperatura y humedad al archivo `Sec30` | El requisito de registro histórico de 24 h corresponde específicamente a estas dos variables | Aprobada |

| DEC-021 | 2026-09-10 | Utilizar Webstation para supervisión desde PC y dispositivos de la red local | Permite acceder a la supervisión mediante navegador web | Aprobada |

| DEC-022 | 2026-09-10 | Habilitar TCP 10008 en Windows Firewall para acceso local a Webstation | Permite conexiones desde otros dispositivos manteniendo activo el firewall del sistema | Aprobada |

