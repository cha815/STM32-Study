#这是我的ESP32学习记录  更新时间12/19

## 1.GPIO输出

** 常用函数 **  pinMode()     digitalWrite()
``` c

//配置引脚
int led_pin = 2;

//set up()
//配置引脚为输出模式
pinMode(led_pin, OUTPUT);

//loop()
//将引脚设置为高电平
digitalWrite(led_pin, HIGH);
//将引脚设置为低电平
digitalWrite(led_pin, LOW);

```

-----------------------------------------------------------------

## 2.一维数码管（分为共阳极和共阴极，自己买的时候注意一下,共阴极需要接高电平才能显示，共阳极反之）

**示例代码**
``` c

//配置引脚--根据实际引脚定义
int pin_a = 2;
int pin_b = 3;
int pin_c = 4;
int pin_d = 5;
int pin_e = 6;
int pin_f = 7;
int pin_g = 8;
int pin_dp = 9;   //dp端是右下角的小数点

//定义数组
int pin[8] = {pin_a, pin_b, pin_c, pin_d, pin_e, pin_f, pin_g, pin_dp};

//定义二维数组（显示数字，共阳极）
int num[10][8] = {
   {0,0,0,0,0,0,1,1},  //0
   {1,0,0,1,1,1,1,1},  //1
   {0,0,1,0,0,1,0,1},  //2
   {0,0,0,0,1,1,0,1},  //3
   {1,0,0,1,1,0,0,1},  //4
   {0,1,0,0,1,0,0,1},  //5
   {0,1,0,0,0,0,0,1},  //6
   {0,0,0,1,1,1,1,1},  //7
   {0,0,0,0,0,0,0,1},  //8
   {0,0,0,1,1,0,0,1}   //9
};
//显示数字的函数
void display(int num)
{
    for(int i = 0; i < 8; i++)
    {
        digitalWrite(pin[i], num[num][i]);
    }
}

void set_up()(
    //配置引脚为输出模式
    for(int i = 0; i < 8; i++)
    {
        pinMode(pin[i], OUTPUT);
    }
    //初始化所有引脚为低电平
    for(int i = 0; i < 8; i++)
    {
        digitalWrite(pin[i], HIGH);
    }
)

void Loop(){
    //循环显示数字
    for(int i = 0; i < 10; i++)
    {
        display(i);
        delay(1000);
    }
}

```
-------------------------------------------------

## 3.四数码管

**基础原理** 四数码管有4条位选线和8条段选线，位选线控制显示哪个数码管，段选线控制数码管显示什么数字。

**示例代码**

``` c

void display_num(int area, int num)
{
    //将所有位选线设置为高电平-----实现清屏的效果
    for(int i = 0; i < 4; i++)
    {
        digitalWrite(bit_pin[i], HIGH);
    }
    //将指定位选线设置为低电平
    digitalWrite(area, LOW);
    //显示数字
    for(int i = 0; i < 8; i++)
    {
        digitalWrite(pin[i], num[num][i]);
    }
}


void scan_display(int num)
{
    //将num转换为数组
    if(num < 10000){
        int num_list[4];
        for(int i = 3; i >= 0; i--)
        {
            num_list[i] = num % 10;
            num /= 10;
        }
        //显示对应位对应数字
         for(int i = 0; i < 4; i++){
            display_num(i, num_list[i]);
            delay(5);   //高频实现同时显示的效果
        } 
    }
}
```

---------------------------------------------------------------------------

## 4.按键消抖

**示例代码**
``` c
//定义引脚
int key_pin = 2;

//定义变量
int key_now = 0;
int key_last = 0;
bool flag = false;

//配置引脚为上拉输入模式（初始状态为高电平）
pinMode(key_pin, INPUT_PULLUP);

//定义一个函数，按键消抖改变标志位
void key_number(){
  key_now = digitalRead(key_pin);
  if(key_now != key_last)
  {
    delay(10);
    key_now = digitalRead(key_pin);
    if(key_now == 0 && key_last == 1)
    {
      flag = !flag;
    }
    key_last = key_now;
  }
}

```
**按键的状态判断**
用户 按下按键

key_now = digitalRead() → 0（因为按下接地）
key_now (0) != key_last (1) → 进入 if 块
delay(10)
再读 key_now → 仍是 0
判断：key_now == 0 && key_last == 1 →  成立 → flag = !flag
更新：key_last = key_now = 0
状态更新完成：key_last = 0

用户 松开按键
key_now = digitalRead() → 1（松开后上拉生效）
key_now (1) != key_last (0) → 进入 if 块（因为电平变了）
delay(10)
再读 key_now → 仍是 1
判断：key_now == 0 && key_last == 1？
→ key_now = 1，所以条件不成立 → 不会翻转 flag
更新：key_last = key_now = 1
状态更新完成：key_last = 1（回到初始状态）

--------------------------------

## 5.pwm输出（版本更新了）

**示例代码**

``` c

analogWrite(pin, value)                 //value范围0-255

analogWriteResolution(bits)             //设置pwm输出的分辨率，默认8位，范围8-12位   12位是2^12=4096级

analogWriteFrequency(pin, freq)         //设置pwm输出的频率，默认5000Hz，范围1-1000000Hz


//LEDC函数（新版本不用配置LEDC通道，直接绑定引脚即可）

ledcAttach(LED, FREQ, RESOLUTION)     //通道，频率，分辨率

ledcWrite(channel, value)             //设置通道的占空比（写入函数）

ledcFade(18, 0, 255, 1000);           //通道，占空比起始值，占空比结束值，渐变时间（单位：毫秒）

ledcDetach(uint8_t pin);                        //解除引脚的 PWM 绑定（旧版叫 ledcDetachPin）。
 
ledcRead(uint8_t pin);                          //读取该引脚当前的占空比。

ledcReadFreq(uint8_t pin);                      //读取该引脚当前的频率。

ledcOutputInvert(uint8_t pin, bool inverted);   //反转 PWM 信号（高低电平对调）。


```

## 6.外部中断

**示例代码**

``` c
#define BOTTUN_PIN 2

void ISR(){
    //处理中断事件
}

//配置引脚为输入模式（上拉）
pinMode(BOTTUN_PIN, INPUT_PULLUP);

//将中断服务函数绑定到引脚
attachInterrupt(digitalPinToInterrupt(BOTTUN_PIN), ISR, FALLING);
//最后一个参数可以是
FALLING  //下降沿触发
RISING   //上升沿触发
CHANGE   //电平变化触发
HIGH     //高电平触发
LOW      //低电平触发

```

## 7.定时器中断（硬件定时器和软件定时器Ticker库）

### 硬件定时器
**示例代码**  

``` c
//硬件定时器

//定义定时器指针
hw_timer_t *timer = NULL;

//初始化定时器  hw_timer_t * timerBegin(uint32_t frequency);
timer = timerBegin(1000);

//绑定中断   void timerAttachInterrupt(hw_timer_t * timer, void (*userFunc)(void));
timerAttachInterrupt(timer, &ISR);

//配置定时时间 void timerAlarm(hw_timer_t * timer, uint64_t alarm_value, bool autoreload, uint64_t reload_count);
//参数分别是定时器指针，定时时间（单位：微秒），是否自动重装载，重装载值（0表示不自动重装载）

timerAlarm(timer, 1000, true, 0);

//其他函数
timerEnd(timer);               //完全销毁并释放定时器资源。

timerRestart(timer);           //将计数值归零并重新开始计数。

timerWrite(timer, val);           //手动设置计数器的当前值。

timerGetFrequency(timer);           //获取定时器当前的实际运行频率。

//定义中断服务函数
void ISR(){
    //处理中断事件
}
```

### 软件定时器（Ticker库）

**示例代码** 
``` c
//包含Ticker库头文件
#include <Ticker.h>

//定义Ticker对象
Ticker LED_Ticker;

//执行函数
void ISR_LED(){
    //处理中断事件
}

//配置定时器相关函数
LED_Ticker.attach_ms(1000, ISR_LED);      //1000ms调用一次ISR_LED函数(周期性调用)

LED_Ticker.attach(1, ISR_LED);           //1s调用一次ISR_LED函数(周期性调用)

LED_Ticker.once_ms(5000, ISR_LED);      //5000ms后调用一次ISR_LED函数(只有一次调用）

LED_Ticker.once(2, ISR_LED);           //2s后调用一次ISR_LED函数(只有一次调用）

LED_Ticker.detach();                      //停止定时器


```