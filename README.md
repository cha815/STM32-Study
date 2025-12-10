# STM32-Study
南京邮电大学Cha的stm32封装学习仓库

📌 显存控制函数（VRAM Control Functions）

函数名	用途
OLED_Clear()	清空显存，清除屏幕上的所有内容
OLED_ClearArea()	清空指定区域的显存
OLED_Reverse()	反转整个屏幕的黑白显示
OLED_ReverseArea()	反转指定区域的黑白显示
📌 显示函数（Display Functions）

函数名	用途
OLED_ShowChar()	显示单个字符
OLED_ShowString()	显示字符串
OLED_ShowNum()	显示无符号数字
OLED_ShowSignedNum()	显示有符号数字（可显示负数）
OLED_ShowHexNum()	显示十六进制数字
OLED_ShowBinNum()	显示二进制数字
OLED_ShowFloatNum()	显示浮点数（整数 + 小数）
OLED_ShowImage()	显示图片/位图
OLED_Printf()	格式化显示，类似 printf
📌 绘图函数（Drawing Functions）

函数名	用途
OLED_DrawPoint()	绘制单个像素点
OLED_GetPoint()	获取某像素点的状态（是否点亮）
OLED_DrawLine()	绘制直线
OLED_DrawRectangle()	绘制矩形（空心/填充）
OLED_DrawTriangle()	绘制三角形（空心/填充）
OLED_DrawCircle()	绘制圆形（空心/填充）
OLED_DrawEllipse()	绘制椭圆（空心/填充）
OLED_DrawArc()	绘制圆弧
