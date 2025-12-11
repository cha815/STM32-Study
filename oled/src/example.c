//示例1------绘制正弦波

/* //引入库
#include "math.h"
   //初始化
   OLED_Init();
   float phase = 0;

  while (1)
  {
   // 清屏
OLED_Clear();

// 画坐标轴
OLED_DrawLine(0, 32, 127, 32); // X 轴
OLED_DrawLine(0, 0, 0, 63);    // Y 轴

// OLED 高度 64，中线在 y=32
// 振幅设为 20（可自己改）
for(int x = 0; x < 128; x++)
{
    // 每个点的实际相位 = 全局相位 + x * 0.1
    float rad = phase + x * 0.1f;
    // 计算正弦波 y 坐标
    int y = 32 + (int)(20 * sinf(rad));
    // 画点
    OLED_DrawPoint(x, y);
}
    // 刷新屏幕
    OLED_Update();
    // 控制波形流动速度(加是向左移动，减是向右边
    phase += 0.1f;                      
    // 刷新频率（你可调）
    HAL_Delay(10);
	OLED_Update();

  }
  */
/*--------------------------------------------------------*/


