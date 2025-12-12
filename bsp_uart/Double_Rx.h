#pragma once
#include "usart.h"

//声明函数

typedef void (*UART_Data_Callback)(uint8_t* data, size_t len);

void STM32_UART_Init (void);
void uart_process(uint8_t* read_buf, uint8_t* write_buf ,size_t len);
void STM32_UART_ISR_Handler_IDLE(UART_HandleTypeDef *huart);
void STM32_UART_RX_ISR_Handler(UART_HandleTypeDef *huart);

void UART_RegisterCallback(UART_Data_Callback callback);