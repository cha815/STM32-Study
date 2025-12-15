# MPU6050_DMP 解算库的调用

## 引入库文件
```c
#include "OLED.h"
#include "OLED_Data.h"
#include "IIC.h"
#include "inv_mpu.h"
#include "mpu6050.h"
#include "inv_mpu_dmp_motion_driver.h"
#include <stdio.h>
```

## 定义变量和缓冲区
```c
float pitch, roll, yaw;
char buffer[50];
```

## 初始化 OLED 和 MPU6050
```c
OLED_Init();
OLED_Clear();
MPU_Init();
mpu_dmp_init();
```

## 获取欧拉角（Yaw/Pitch/Roll）
```c
mpu_dmp_get_data(&pitch, &roll, &yaw);
```

## 将数据格式化并显示到 OLED
```c
sprintf((char *)buffer, "P:%.2f  ", pitch);
OLED_ShowString(0, 0, (char *)buffer, 8);

sprintf((char *)buffer, "Y:%.2f  ", yaw);
OLED_ShowString(0, 17, (char *)buffer, 8);

sprintf((char *)buffer, "R:%.2f  ", roll);
OLED_ShowString(0, 33, (char *)buffer, 8);
```

## 修改软件 I2C（IIC）引脚
如果需要修改软件 IIC 使用的引脚，请在 `IIC.c` 中调整下面 4 行定义：
```c
/* 定义IIC总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
#define GPIO_PORT_IIC     GPIOA                       /* GPIO端口 */
#define RCC_IIC_ENABLE    __HAL_RCC_GPIOB_CLK_ENABLE() /* GPIO端口时钟 */
#define IIC_SCL_PIN       GPIO_PIN_0                  /* 连接到SCL时钟线的GPIO */
#define IIC_SDA_PIN       GPIO_PIN_1                  /* 连接到SDA数据线的GPIO */
```

## 刷新 OLED
```c
OLED_Update();
```

> 说明：以上示例使用 `sprintf` 将浮点数格式化为字符串并通过 `OLED_ShowString` 显示，使用前请确保 `printf`/`sprintf` 的库在工程中被正确支持（有时需要启用 