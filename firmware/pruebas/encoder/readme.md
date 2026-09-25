# Prueba de encoder incremental

## Hardware

Encoder utilizado: **Omron E6B2-CWZ6C, 1000 P/R**, con salidas NPN open collector.

Conexiones verificadas:

- alimentación: 5 V;
- canal A: GPIO32;
- canal B: GPIO33;
- pull-up externo de 4.7 kΩ a 3.3 V en A y B;
- fase Z: no utilizada.

## Estrategia de adquisición

El firmware de prueba utiliza interrupciones sobre ambos flancos de la fase A (`CHANGE`) y consulta el estado de la fase B para determinar el sentido de giro.

Con 1000 P/R y dos flancos utilizados por pulso:

```text
1000 × 2 = 2000 cuentas/revolución
360° / 2000 = 0.18°/cuenta
```

## Verificaciones realizadas

Se comprobó:

- incremento y decremento según sentido de giro;
- estabilidad con el eje detenido;
- retorno aproximadamente al origen tras una revolución completa hasta una marca física de referencia;
- integración posterior del contador y del ángulo en el firmware principal y en Modbus.

La fase Z no forma parte de la implementación actual, por lo que la referencia angular al encender corresponde a la posición inicial del eje.

## Estado

**APROBADO.**
