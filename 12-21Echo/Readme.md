# 超声波测距实验

## CUBEMX配置

1.   配置超声波传感器引脚
2.   Trig引脚配置普通的GPIO_OUTPUT
3.   Echo引脚配置TIM_输入捕获通道(直接捕获)
4.   然后对应的在隔壁TIM通道输入捕获通道(间接捕获)
5.   在NVIC中使能 TIM_capture compare interrupt
6.   主频/分频 = 定时器计数频率
     例如：主频72MHz，分频72，那么定时器计数频率就是1MHz
7.   不开启自动重装载
8.   为直接捕获配置上升沿检测,间接捕获配置下降沿检测

## 代码实现

``` c
    //启动通道
    HAL_TIM_IC_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_4);
    /* while 里的代码 */
    //产生超声波脉冲
    HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, GPIO_PIN_RESET);
    //清空计数器
    __HAL_TIM_SET_COUNTER(&htim1, 0); 
    //延迟
    HAL_Delay(20); 
    //OLED显示
    OLED_ShowNum(0, 0, avg_distance_cm, 3, 8);
    OLED_ShowString(0, 16, "cm", 8);
    OLED_Update();

```
### 重定义捕获中断函数

``` c
// 捕获中断回调函数 + 简单平均值滤波
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{ avg_distance_cm = 0;
  if(htim->Instance == TIM1 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
  {
    for (int i = 0; i < SAMPLE_RATE; i++)
    {
      RISE_EDGE[i] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
      FALL_EDGE[i] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
      if (RISE_EDGE[i] < FALL_EDGE[i]){
      DISTANCE[i] = FALL_EDGE[i] - RISE_EDGE[i];
      distance_cm[i] = DISTANCE[i] * 0.034 / 2.0f;
      avg_distance_cm += distance_cm[i];}
      }
    avg_distance_cm /= SAMPLE_RATE;
  }

  }

```