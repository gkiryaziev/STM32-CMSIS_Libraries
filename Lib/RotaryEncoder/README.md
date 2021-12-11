## Rotary Encoder (polling method with debounce)

Simple example.

#### Usage
```c
RotaryEncoder_TypeDef encoder0;

RotaryEncoder_Init(&encoder0, GPIOB, 3, GPIOB, 4);

int8_t encoder0_value = RotaryEncoder_Read(&encoder0)
```

```
1 - CW - d42b
2 - CW - d42b
3 - CW - d42b
4 - CW - d42b
5 - CW - d42b
6 - CW - d42b
7 - CW - d42b
8 - CW - d42b
9 - CW - d42b
8 - CCW - e817
7 - CCW - e817
6 - CCW - e817
5 - CCW - e817
4 - CCW - e817
3 - CCW - e817
2 - CCW - e817
1 - CCW - e817
0 - CCW - e817
```

![Alt text](encoder_module.png?raw=true "encoder_module")

*All libraries are under development and are provided as is.*
