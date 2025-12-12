# UART HAL 常用函数与回调（带代码示例）


### （1）HAL_UART_Transmit（轮询阻塞式发送）

``` c
HAL_UART_Transmit(&huart4, tx_buf, tx_len, 100);

/*参数  uart句柄，发送的信息，发送长度，超时时间*/
```

**使用场景** - 调试发送（printf、日志） - 数据量少 - 非实时场景


------------------------------------------------------------------------

### （2）HAL_UART_Transmit_IT（中断发送）

``` c
HAL_UART_Transmit_IT(&huart4, tx_buf, tx_len);

/*参数  uart句柄，发送的信息，发送长度*/
```

**使用场景**  配合HAL_UART_TxCpltCallbac回调函数，提醒接收完成发送下一段数据

``` c
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == &huart4)
        tx_flag = 1;            //触发中断，标志位置1，表示有一种数据已发送
}

/*在主循环中的代码*/
if (tx_flag == 1)
{
    tx_flag = 0;                //标志位清零并发送下一帧数据
    HAL_UART_Transmit_DMA(&huart4, buf, len);
}

```

------------------------------------------------------------------------


### （3）HAL_UART_Transmit_DMA（DMA 发送）

``` c
HAL_UART_Transmit_DMA(&huart4, tx_buf, tx_len);
```

**使用场景（推荐）**  大量发送

**回调** 同样触发 `TxCpltCallback`。


------------------------------------------------------------------------


### （4）HAL_UART_Receive（阻塞接收）

``` c

HAL_UART_Receive(&huart4, rx_buf, 10, 100);

/*参数  uart句柄，接收缓冲区，接收长度，超时时间*/

```

**使用场景** - 固定长度数据，堵塞接收


------------------------------------------------------------------------

### （5）HAL_UART_Receive_IT（中断接收）

``` c

HAL_UART_Receive_IT(&huart4, rx_buf, 10);

/*参数  uart句柄，接收缓冲区，接收长度*/

```

**使用场景** - 固定长度，如 10 字节协议

**回调**

``` c

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart4)
    {
        process(rx_buf, 10);                       //上层建筑，对收取到的10字节数据进行操作
        HAL_UART_Receive_IT(&huart4, rx_buf, 10);    
    }
}

```

------------------------------------------------------------------------

### （6）HAL_UART_Receive_DMA（DMA 接收）

``` c

HAL_UART_Receive_DMA(&huart4, rx_buf, RX_BUF_SIZE);

/*参数  uart句柄，接收缓冲区，接收长度*/

```

**使用场景**  接收不定长数据，配合 IDLE 中断判断一帧结束

------------------------------------------------------------------------

### （7）HAL_UARTEx_ReceiveToIdle_DMA (DMA+IDLE)

``` c

HAL_UARTEx_ReceiveToIdle_DMA(&huart4, rx_buf, RX_LEN);

/* 参数  uart句柄，接收缓冲区，接收长度*/

```
**使用场景** 开启 UART DMA 接收，并在接收到数据且线路空闲时自动触发回调，（HAL_UARTEx_RxEventCallback），非常适合接收不定长度数据。

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

##  UART 回调函数（附代码）

### （1）HAL_UART_TxCpltCallback（发送完成）

``` c
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == &huart4)
        tx_done = 1;
}
```

------------------------------------------------------------------------

### （2）HAL_UART_RxCpltCallback（IT 接收完成）

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

### （3）HAL_UART_RxHalfCpltCallback（DMA 半满）

``` c
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
    // rx_buf[0 ~ half] 可处理
}
```

------------------------------------------------------------------------

### （4）HAL_UARTEx_RxEventCallback（最推荐）

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

### （5）HAL_UART_ErrorCallback（错误处理）

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

##  最推荐 DMA + IDLE 完整模板

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

##  必学函数总结

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
