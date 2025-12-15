#pragma once

#include "stdint.h"
#include "stddef.h"

typedef void (*CAN_Data_Callback)(uint8_t* data, size_t len);               //定义一种函数指针类型
void CAN_cmd(int16_t encoder, int16_t speed, int16_t current, uint8_t temperature);
void CAN_RegisterCallback(CAN_Data_Callback callback);
void can_filter_init(void);