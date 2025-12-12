//12/11号，与uart有关的程序编写和库封装


//1--快速检查串口发送是否正常
/*  
    char buf[20];
    int i = 0;
    i++;
    sprintf(buf,"Num:%d",i);
    HAL_UART_Transmit(&huart1,(uint8_t *)buf,strlen(buf),1000);
    HAL_Delay(1000);

*/

//2--Double_Rx库使用示例
/*步骤1，定义一个UART_Data_Callback类型的函数(返回类型是这个函数的指针），例如：

  UART_Data_Callback UART_Data_Handler(uint8_t* data, size_t len){

    if(len == 10){
			dbus_parseData(&dbus,data);  //接收键鼠信息（10字节）			
		}
		whl++;        （记录接收次数，是否接收成功）
    return NULL;
}
步骤2，调用注册回调函数：（初始化一次就可以了）INIT

  STM32_UART_Init();
  UART_RegisterCallback(UART_Data_Handler);

*/



