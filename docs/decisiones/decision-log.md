# Registro de decisiones de diseño

| ID | Fecha | Decisión | Justificación | Estado |
|---|---|---|---|---|
| DEC-001 | 2026-08-31 | Evaluar la Waveshare ESP32-S3-POE-ETH-8DI-8DO como plataforma principal | Plataforma provista inicialmente por la cátedra | Cerrada |
| DEC-002 | 2026-08-31 | Utilizar RS485 como medio de comunicación | Requisito definido para la práctica | Aprobada |
| DEC-003 | 2026-09-01 | Utilizar GPIO27 para DHT11 en ESP32 NodeMCU | Funcionamiento verificado experimentalmente | Aprobada |
| DEC-004 | 2026-09-01 | Utilizar `dhtESP32-rmt` para adquisición DHT11 | Funcionamiento estable verificado sobre ESP32 | Aprobada |
| DEC-005 | 2026-09-01 | Utilizar ESP32 NodeMCU como nodo principal de adquisición | Sensores y comunicación fueron verificados sobre GPIO directos; la cátedra aceptó su utilización | Aprobada |
| DEC-006 | 2026-09-02 | Utilizar MAX485 disponible para RS485 | Permite implementar la interfaz física requerida incorporando adaptación de nivel en RO | Aprobada |
| DEC-007 | 2026-09-02 | Utilizar GPIO17, GPIO16 y GPIO4 para RS485 | Configuración verificada mediante comunicación ESP32-PC | Aprobada |
| DEC-008 | 2026-09-02 | Utilizar ESP32 como Modbus RTU Slave ID 1 durante el desarrollo individual | Comunicación verificada mediante Modbus Poll y Rapid SCADA | Aprobada |
| DEC-009 | 2026-09-04 | Compensar la medición ultrasónica con la temperatura del DHT11 | La velocidad del sonido depende de la temperatura y la compensación es requisito de la práctica | Aprobada |
| DEC-010 | 2026-09-04 | Conservar distancia sin compensar y compensada en Modbus | Permite evaluar el efecto de la compensación | Aprobada |
| DEC-011 | 2026-09-09 | Utilizar GPIO32 y GPIO33 para fases A y B del encoder | Pines disponibles y funcionamiento verificado | Aprobada |
| DEC-012 | 2026-09-09 | Representar la posición angular entre 0° y 360° | Responde al requisito de adquirir posición angular | Aprobada |
| DEC-013 | 2026-09-09 | Utilizar 2000 cuentas por revolución | Se detectan ambos flancos de A en un encoder de 1000 P/R | Aprobada |
| DEC-014 | 2026-09-09 | Transmitir contador del encoder en dos registros de 16 bits | Conserva el contador de 32 bits con signo | Aprobada |
| DEC-015 | 2026-09-10 | Configurar Rapid SCADA Communicator como maestro Modbus RTU sobre COM8 durante las pruebas del grupo | Integra el nodo usando 9600 8N1 previamente validado | Aprobada |
| DEC-016 | 2026-09-10 | Leer HR0-HR6 como bloque contiguo mediante FC03 | Permite adquirir las siete variables con una petición | Aprobada |
| DEC-017 | 2026-09-10 | Utilizar canales 101-107 en Rapid SCADA | Asocia los registros con variables identificables | Aprobada |
| DEC-018 | 2026-09-10 | Aplicar `Cnl * 0.1` a variables transmitidas ×10 | Recupera las unidades físicas en SCADA | Aprobada |
| DEC-019 | 2026-09-10 | Crear archivo histórico `Sec30` con período de 30 s | Cumple el requisito de registro periódico | Aprobada |
| DEC-020 | 2026-09-10 | Asociar temperatura y humedad a `Sec30` | Son las variables requeridas para el registro prolongado | Aprobada |
| DEC-021 | 2026-09-10 | Utilizar Webstation para supervisión web | Permite acceso mediante navegador | Aprobada |
| DEC-022 | 2026-09-10 | Habilitar TCP 10008 en Windows Firewall | Permite acceso a Webstation manteniendo activo el firewall | Aprobada |
| DEC-023 | 2026-09-16 | Utilizar VPN para verificar acceso remoto a Webstation | La cátedra confirmó que el acceso debe realizarse por Internet y que una VPN es válida | Aprobada |
| DEC-024 | 2026-09-18 | Restringir el acceso remoto compartido al host SCADA y TCP 10008 | Aplica mínimo privilegio sin Exit Node ni Subnet Router | Aprobada |
| DEC-025 | 2026-09-16 | Eliminar del alcance comandos de posición relativa a un robot | La cátedra confirmó que esa referencia era un error del enunciado | Cerrada |
| DEC-026 | 2026-09-18 | Mantener compensación térmica sin corrección empírica adicional | La caracterización con LM50A mostró mejora y se prefirió documentar el error observado | Aprobada |
| DEC-027 | 2026-09-25 | Conservar como datos faltantes las interrupciones del ensayo prolongado | No se determinó concluyentemente su causa; no se interpolan ni sustituyen mediciones ausentes | Aprobada |
| DEC-028 | 2026-09-25 | Mantener pendiente la topología común entre grupos hasta definición de la cátedra | Evita documentar como implementada una arquitectura aún no confirmada | Pendiente |
