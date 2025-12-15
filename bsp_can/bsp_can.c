#include "bsp_can.h"
#include "can.h"


CAN_TxHeaderTypeDef tx_message;                               //定义发送头文件的变量
uint8_t can_send_data[8];                                     //定义发送数据的数组
static CAN_Data_Callback can_data_callback = NULL;            //定义回调函数指针
extern CAN_HandleTypeDef hcan;                                  


void can_filter_init(void){
    CAN_FilterTypeDef can_filter_st;
    can_filter_st.FilterActivation = ENABLE;                            //开启滤波器
    can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;                   //ID掩码模式
    can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;                  //32位滤波器
    can_filter_st.FilterIdHigh = 0x0000;                                //无论什么ID都接收
    can_filter_st.FilterIdLow = 0x0000;
    can_filter_st.FilterMaskIdHigh = 0x0000;                            //全接收模式（掩码）
    can_filter_st.FilterMaskIdLow = 0x0000;
    can_filter_st.FilterBank = 0;                                       //使用滤波器0
    can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;                  //将接收的消息存储到FIFO0
    HAL_CAN_ConfigFilter(&hcan, &can_filter_st);                        //应用滤波器配置
    HAL_CAN_Start(&hcan);                                               //启动CAN外设
    HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);   //使能FIFO0消息挂起中断
}


//发送函数
void CAN_cmd(int16_t encoder, int16_t speed, int16_t current, uint8_t temperature)//-32768-32768//10000
{
    uint32_t send_mail_box;
    tx_message.StdId = 0x201;
    tx_message.IDE = CAN_ID_STD;
    tx_message.RTR = CAN_RTR_DATA;
    tx_message.DLC = 0x08;
    can_send_data[0] = (encoder >> 8);
    can_send_data[1] = encoder;
    can_send_data[2] = (speed >> 8);
    can_send_data[3] = speed;
    can_send_data[4] = (current >> 8);
    can_send_data[5] = current;
    can_send_data[6] = temperature;
    can_send_data[7] = 0x00;
    HAL_CAN_AddTxMessage(&hcan, &tx_message, can_send_data, &send_mail_box);
}


//接收回调函数（从fifo0队列中接收）
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);

    if (rx_header.IDE ==  CAN_ID_STD){
        switch (rx_header.StdId){
        case 0x200:
                if (can_data_callback != NULL) {
                    can_data_callback(rx_data, rx_header.DLC);
                }
                break;
        default:
                break;

        }
    }
}


// 注册回调函数（应用层)
void CAN_RegisterCallback(CAN_Data_Callback callback){
    can_data_callback = callback;
}