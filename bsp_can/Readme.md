# CAN 通信常见函数介绍（速查版）

> 面向 **STM32 + HAL 库** 的 CAN 通信函数总结，适合学习、调试和比赛快速查阅。

---

## 一、CAN 通信基本流程概览

```text
CAN 初始化
   ↓
过滤器配置
   ↓
启动 CAN 外设
   ↓
发送 / 接收数据
   ↓
中断 / 回调处理
```

---

## 二、CAN 初始化相关函数

### 1?  `HAL_CAN_Init`

```c
HAL_CAN_Init(&hcan);
```

**作用：**

* 初始化 CAN 外设
* 配置波特率、工作模式、时间段等

**常见参数配置（CubeMX 生成）：**

* Prescaler（分频系数）
* Mode（Normal / LoopBack / Silent）
* BS1 / BS2（位时间）

---

### 2?  `HAL_CAN_Start`

```c
HAL_CAN_Start(&hcan);
```

**作用：**

* 启动 CAN 模块
* 启用后才能发送和接收数据

---

## 三、CAN 过滤器配置函数

### 3?  `HAL_CAN_ConfigFilter`

```c
HAL_CAN_ConfigFilter(&hcan, &canFilter);
```

**作用：**

* 配置 CAN 接收过滤器
* 决定哪些 ID 的报文可以进入 FIFO

**常见配置（全接收）：**

```c
canFilter.FilterMode = CAN_FILTERMODE_IDMASK;
canFilter.FilterScale = CAN_FILTERSCALE_32BIT;
canFilter.FilterIdHigh = 0x0000;
canFilter.FilterMaskIdHigh = 0x0000;
```

---

## 四、CAN 发送相关函数

### 4?  `HAL_CAN_AddTxMessage`

```c
HAL_CAN_AddTxMessage(&hcan, &txHeader, txData, &mailbox);
```

**作用：**

* 将一帧 CAN 数据放入发送邮箱

**关键结构体：**

```c
CAN_TxHeaderTypeDef txHeader;
```

常用成员：

* `StdId`：标准帧 ID（11 位）
* `ExtId`：扩展帧 ID（29 位）
* `IDE`：CAN_ID_STD / CAN_ID_EXT
* `RTR`：CAN_RTR_DATA / CAN_RTR_REMOTE
* `DLC`：数据长度（0~8）

---

### 5?  `HAL_CAN_IsTxMessagePending`

```c
HAL_CAN_IsTxMessagePending(&hcan, mailbox);
```

**作用：**

* 判断发送邮箱是否还在发送中

---

## 五、CAN 接收相关函数

### 6 `HAL_CAN_GetRxMessage`

```c
HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &rxHeader, rxData);
```

**作用：**

* 从 FIFO 中读取一帧 CAN 数据

**关键结构体：**

```c
CAN_RxHeaderTypeDef rxHeader;
```

常用成员：

* `StdId / ExtId`
* `IDE`
* `RTR`
* `DLC`

---

## 六、CAN 中断与回调函数 ?

### 7?  `HAL_CAN_ActivateNotification`

```c
HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
```

**作用：**

* 使能 CAN 中断通知

常用中断类型：

* `CAN_IT_RX_FIFO0_MSG_PENDING`
* `CAN_IT_TX_MAILBOX_EMPTY`

---

### 8?  `HAL_CAN_RxFifo0MsgPendingCallback`

```c
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, rxData);
}
```

**作用：**

* 接收中断回调函数
* 当 FIFO0 有新消息时自动调用

---

### 9?  `HAL_CAN_TxMailbox0CompleteCallback`

```c
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
```

**作用：**

* 发送完成回调

---

## 七、CAN 状态与错误处理

### 10?  `HAL_CAN_GetState`

```c
HAL_CAN_GetState(&hcan);
```

**作用：**

* 获取 CAN 当前状态

---

### 11?  `HAL_CAN_ErrorCallback`

```c
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
```

**作用：**

* CAN 出现错误时的回调

---

## 八、常见调试建议

* 12? 先确认 **波特率一致**
* 13? 至少两节点（CAN 无自发自收，除非回环模式）
* 14? 优先使用 **中断接收**
* 15? 过滤器先全开，稳定后再精确过滤

---

## 九、典型发送 + 接收示例（简化）

```c
// 发送
HAL_CAN_AddTxMessage(&hcan, &txHeader, txData, &mailbox);

// 接收（中断）
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, rxData);
}
```

