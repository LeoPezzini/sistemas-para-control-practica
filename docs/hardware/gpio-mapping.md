# Mapeo de GPIO

## ESP32 NodeMCU

El ESP32 NodeMCU se utiliza como nodo principal de adquisición, procesamiento y comunicación Modbus RTU.

La asignación de GPIO implementada es la siguiente:

| Dispositivo | Señal | GPIO | Estado | Observaciones |
|---|---|---:|---|---|
| DHT11 | DATA | 27 | Verificado | Alimentación a 3.3 V |
| HC-SR04 | TRIG | 25 | Verificado | Salida directa desde ESP32 |
| HC-SR04 | ECHO | 26 | Verificado | Entrada mediante divisor 1 kΩ / 2 kΩ |
| MAX485 | DI / TX | 17 | Verificado | UART2 TX |
| MAX485 | RO / RX | 16 | Verificado | UART2 RX mediante divisor 1 kΩ / 2 kΩ |
| MAX485 | DE + /RE | 4 | Verificado | Control de dirección half-duplex |
| Encoder | Canal A | 32 | Verificado | Pull-up externo de 4.7 kΩ a 3.3 V |
| Encoder | Canal B | 33 | Verificado | Pull-up externo de 4.7 kΩ a 3.3 V |

## Alimentación y adaptación de niveles

Todos los dispositivos utilizados comparten una referencia de GND común.

### DHT11

Conexiones:

- VCC: 3.3 V;
- GND: GND común;
- DATA: GPIO27.

El sensor se utiliza para adquirir temperatura y humedad relativa. La temperatura obtenida también interviene en la compensación de la velocidad del sonido utilizada para el HC-SR04.

### HC-SR04

Conexiones:

- VCC: 5 V;
- GND: GND común;
- TRIG: GPIO25;
- ECHO: GPIO26 mediante divisor resistivo.

La salida ECHO del HC-SR04 trabaja a un nivel asociado a su alimentación de 5 V, por lo que se utiliza un divisor resistivo antes de ingresar al GPIO26 del ESP32.

El divisor implementado es:

```text
HC-SR04 ECHO
     │
    1 kΩ
     │
     ├──────── GPIO26
     │
    2 kΩ
     │
    GND
```

La relación ideal del divisor es:

```text
VGPIO = VECHO × 2 kΩ / (1 kΩ + 2 kΩ)
```

Para una entrada de 5 V:

```text
VGPIO ≈ 3.33 V
```

De esta forma se evita aplicar directamente una señal de aproximadamente 5 V a la entrada del ESP32.

### MAX485

El módulo MAX485 se alimenta a 5 V.

Conexiones:

- DI: GPIO17 / TX2;
- RO: GPIO16 / RX2 mediante divisor resistivo;
- DE y /RE: GPIO4;
- VCC: 5 V;
- GND: GND común.

La salida RO también se adapta mediante un divisor resistivo de 1 kΩ / 2 kΩ antes de ingresar al GPIO16.

Durante las pruebas se midieron aproximadamente:

- 4.57 V entre RO y GND antes del divisor;
- 3.18 V entre GPIO16 y GND después del divisor.

La comunicación RS485 fue verificada experimentalmente utilizando esta configuración.

### Encoder incremental

Se utiliza un encoder Omron E6B2-CWZ6C de 1000 P/R con salidas NPN open collector.

Conexiones utilizadas:

- alimentación: 5 V;
- GND: GND común;
- canal A: GPIO32;
- canal B: GPIO33;
- fase Z: no utilizada.

Los canales A y B utilizan resistencias pull-up externas de 4.7 kΩ conectadas a 3.3 V.

```text
3.3 V                  3.3 V
  │                      │
4.7 kΩ                  4.7 kΩ
  │                      │
  ├──── GPIO32           ├──── GPIO33
  │                      │
Canal A                Canal B
Encoder                Encoder
```

Al tratarse de salidas open collector, las resistencias pull-up permiten obtener niveles lógicos compatibles con las entradas del ESP32.

## Resumen de tensiones

| Elemento | Alimentación | Nivel hacia ESP32 |
|---|---:|---:|
| DHT11 | 3.3 V | 3.3 V |
| HC-SR04 | 5 V | ECHO adaptado a aproximadamente 3.3 V |
| MAX485 | 5 V | RO adaptado a aproximadamente 3.3 V |
| Encoder | 5 V | Pull-up externo a 3.3 V |

## Estado

Las conexiones indicadas fueron implementadas y verificadas experimentalmente en el sistema integrado.
