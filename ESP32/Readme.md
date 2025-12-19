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

** 基础知识 ** 四数码管有4条位选线和8条段选线，位选线控制显示哪个数码管，段选线控制数码管显示什么数字。

** 示例代码 **

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