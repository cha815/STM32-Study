//12/12号，uart双缓冲接收程序编写

/*如果用的是其他接口，更改STM32_UART_Init里的句柄就行了*/

/*--------------------------------------------------------------------------------------*/
#include "Double_Rx.h"
#include "string.h"

#define DMA_SIZE 256
#define USER_LEN 512
uint8_t uart1_rx_buf[DMA_SIZE] = {0};
uint8_t user_buf[USER_LEN] = {0};
uint8_t process_buf[USER_LEN] = {0};
size_t user_buf_len = 0;

// 回调函数指针
static UART_Data_Callback uart_data_callback = NULL;

void STM32_UART_Init (void){
    __HAL_UART_ENABLE_IT(&huart4,UART_IT_IDLE);                 //使能空闲中断
    HAL_UART_Receive_DMA(&huart4,uart1_rx_buf,sizeof(uart1_rx_buf));        //使能DMA接收中断
}

void STM32_UART_RX_ISR_Handler(UART_HandleTypeDef *huart){
    static size_t curr_pos = 0;
    static size_t last_pos = 0;
    size_t dma_size = DMA_SIZE;                                    //DMA写入数组的大小
    curr_pos = dma_size - __HAL_DMA_GET_COUNTER(huart->hdmarx);    //获取当前DMA写入位置

    if (curr_pos != last_pos) {
        if (curr_pos > last_pos) {
            // 线性区域，直接拷贝//last_pos = 0 curr_pos = 10
            memcpy(&user_buf[user_buf_len], &uart1_rx_buf[last_pos], curr_pos - last_pos);
            uart_process(&process_buf[0],&user_buf[user_buf_len],curr_pos - last_pos);
            user_buf_len += curr_pos - last_pos;
            // 如果注册了回调函数，调用回调函数传递数据
            if (uart_data_callback != NULL) {
                uart_data_callback(&process_buf[0], curr_pos - last_pos);
            }

        } else {
            // 环形区跨越    last_pos = 250 curr_pos = 4
            memcpy(&user_buf[user_buf_len], &uart1_rx_buf[last_pos], dma_size - last_pos);
            uart_process(&process_buf[0],&user_buf[user_buf_len],dma_size - last_pos);
            user_buf_len = 0;
            memcpy(&user_buf[user_buf_len], &uart1_rx_buf[0], curr_pos);
            uart_process(&process_buf[dma_size - last_pos],&user_buf[user_buf_len], curr_pos);
            user_buf_len += curr_pos;
            // 调用回调函数
            if (uart_data_callback != NULL) {
                uart_data_callback(&process_buf[0], dma_size - last_pos + curr_pos);
            }
        }
        last_pos = curr_pos; // 更新上次位置
    }
}

void uart_process(uint8_t* read_buf, uint8_t* write_buf ,size_t len){
    memcpy(read_buf,write_buf,len);
}

// 注册回调函数
void UART_RegisterCallback(UART_Data_Callback callback){
    uart_data_callback = callback;
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart){//半满中断
    if (huart == &huart4){
        STM32_UART_RX_ISR_Handler(huart);

    }
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){    //全满中断
    if (huart == &huart4){
        STM32_UART_RX_ISR_Handler(huart);
    }
}
void STM32_UART_ISR_Handler_IDLE(UART_HandleTypeDef *huart){//空闲中断
    if (__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE)){
        __HAL_UART_CLEAR_IDLEFLAG(huart);
        STM32_UART_RX_ISR_Handler(huart);
    }
}

