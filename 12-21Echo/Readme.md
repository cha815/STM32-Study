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
    //清空计数器---中断有回环处理可以不清空
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

//定义变量
#define SAMPLE_RATE 5
uint16_t RISE_EDGE = 0;
uint16_t FALL_EDGE = 0;
uint16_t DISTANCE = 0;
uint16_t avg_distance_cm = 0;

float history_index[SAMPLE_RATE] = {0};  //样本缓存索引
uint16_t buffer_index = 0;  //当前样本点存放索引

// 捕获中断回调函数 + 简单平均值滤波
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM1 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
  {
    // 读取捕获值
      RISE_EDGE = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
      FALL_EDGE = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
    // 计算距离（防止回环）
    if (RISE_EDGE < FALL_EDGE) {DISTANCE = FALL_EDGE - RISE_EDGE;}
    else{DISTANCE = (0xFFFF - RISE_EDGE) + FALL_EDGE;}
    //存放入缓存
    history_index[buffer_index] = (float)DISTANCE * 0.034 /2;
    buffer_index++;
    if (buffer_index >= SAMPLE_RATE)buffer_index = 0; //10个是一个周期
    //计算平均值
     float sum = 0;
    for (uint8_t i = 0; i < SAMPLE_RATE; i++)
    {
      sum += history_index[i];
    }
    avg_distance_cm = sum / SAMPLE_RATE;
  }

  }

```