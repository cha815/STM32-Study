# UART HAL 常用函数与回调（带代码示例）

## ? 1. UART 发送常用 HAL 函数（附代码）

### ?（1）HAL_UART_Transmit（轮询阻塞式发送）

``` c
HAL_UART_Transmit(&huart4, tx_buf, tx_len, 100);
```

**使用场景** - 调试发送（printf、日志） - 数据量少 - 非实时场景

**特点** - 阻塞直到发送完成\
- 简单，但不适合高频大量发送

------------------------------------------------------------------------

### ?（2）HAL_UART_Transmit_IT（中断发送）

``` c
HAL_UART_Transmit_IT(&huart4, tx_buf, tx_len);
```

**使用场景** - 小数据包\
- 主动发送，不想阻塞

**回调**

``` c
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == &huart4)
        tx_done = 1;
}
```

------------------------------------------------------------------------

### ??（3）HAL_UART_Transmit_DMA（DMA 发送）

``` c
HAL_UART_Transmit_DMA(&huart4, tx_buf, tx_len);
```

**使用场景（推荐）** - 大量发送\
- 实时性高\
- CPU 占用率低\
- 比赛、机器人、电控推荐 DMA 发送

**回调** 同样触发 `TxCpltCallback`。

------------------------------------------------------------------------

## ? 2. UART 接收常用 HAL 函数（附代码）

### ?（1）HAL_UART_Receive（阻塞）

``` c
HAL_UART_Receive(&huart4, rx_buf, 10, 100);
```

**使用场景** - 固定长度数据\
- 不适合实时系统

------------------------------------------------------------------------

### ?（2）HAL_UART_Receive_IT（中断接收）

``` c
HAL_UART_Receive_IT(&huart4, rx_buf, 10);
```

**使用场景** - 固定长度，如 10 字节协议

**回调**

``` c
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart4)
    {
        process(rx_buf, 10);
        HAL_UART_Receive_IT(&huart4, rx_buf, 10);
    }
}
```

------------------------------------------------------------------------

### ???（3）HAL_UART_Receive_DMA（DMA 接收）

``` c
HAL_UART_Receive_DMA(&huart4, rx_buf, RX_BUF_SIZE);
```

**使用场景** - 不固定长度数据\
- 大量接收\
- 常配合 IDLE 中断判断一帧结束

------------------------------------------------------------------------

### ?????（4）HAL_UARTEx_ReceiveToIdle_DMA（最推荐）

``` c
HAL_UARTEx_ReceiveToIdle_DMA(&huart4, rx_buf, RX_LEN);
```

**回调**

``` c
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
{
    if (huart == &huart4)
    {
        process(rx_buf, size);  
        HAL_UARTEx_ReceiveToIdle_DMA(&huart4, rx_buf, RX_LEN);
    }
}
```

------------------------------------------------------------------------

## ? 3. UART 回调函数（附代码）

### ??（1）HAL_UART_TxCpltCallback（发送完成）

``` c
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == &huart4)
        tx_done = 1;
}
```

------------------------------------------------------------------------

### ??（2）HAL_UART_RxCpltCallback（IT 接收完成）

``` c
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == &huart4)
    {
        process(rx_buf, EXPECT_LEN);
        HAL_UART_Receive_IT(&huart4, rx_buf, EXPECT_LEN);
    }
}
```

------------------------------------------------------------------------

### ?（3）HAL_UART_RxHalfCpltCallback（DMA 半满）

``` c
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
    // rx_buf[0 ~ half] 可处理
}
```

------------------------------------------------------------------------

### ????（4）HAL_UARTEx_RxEventCallback（最推荐）

``` c
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
{
    if (huart == &huart4)
    {
        process(rx_buf, size);
        HAL_UARTEx_ReceiveToIdle_DMA(&huart4, rx_buf, RX_LEN);
    }
}
```

------------------------------------------------------------------------

### ???（5）HAL_UART_ErrorCallback（错误处理）

``` c
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if(huart == &huart4)
    {
        __HAL_UART_CLEAR_OREFLAG(huart);  
        HAL_UARTEx_ReceiveToIdle_DMA(&huart4, rx_buf, RX_LEN);
    }
}
```

------------------------------------------------------------------------

## ? 4. 最推荐 DMA + IDLE 完整模板

``` c
uint8_t rx_buf[256];

void UART_Init(void)
{
    HAL_UARTEx_ReceiveToIdle_DMA(&huart4, rx_buf, sizeof(rx_buf));
    __HAL_DMA_DISABLE_IT(huart4.hdmarx, DMA_IT_HT);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
{
    if(huart == &huart4)
    {
        process(rx_buf, size);
        HAL_UARTEx_ReceiveToIdle_DMA(&huart4, rx_buf, sizeof(rx_buf));
    }
}
```

------------------------------------------------------------------------

## ? 必学函数总结

### **发送（常用程度排序）**

1.  `HAL_UART_Transmit_DMA`（最常用）
2.  `HAL_UART_Transmit_IT`
3.  `HAL_UART_Transmit`

### **接收（最推荐）**

1.  `HAL_UARTEx_ReceiveToIdle_DMA` + `RxEventCallback`
2.  `HAL_UART_Receive_DMA` + IDLE（旧写法）
3.  `HAL_UART_Receive_IT`（固定长度）

### **主要回调**

-   `HAL_UARTEx_RxEventCallback`
-   `HAL_UART_ErrorCallback`
-   `HAL_UART_TxCpltCallback`
-   `HAL_UART_RxCpltCallback`
