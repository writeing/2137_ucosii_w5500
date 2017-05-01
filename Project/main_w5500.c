

/*
 http://zq281598585.taobao.com/  
启光电子	  ELH    enlighten  sunny

w5500网络模块  参考程序

运行测试MCU STM32F103RBT6


TCP客户端模式 

中断判断方式
TCP客户端是将模块自己设置为客户，然后和服务器端进行数据通讯
因此此模式要确定模块自己的IP、端口和目标服务器的IP、端口


W5500所有引脚对应如下： 
sck PA5  miso PA6  mosi PA7  rst PA2  int PA3  cs PA0



测试路由各数据值

网关：192.168.0.1		   （其为我们测试路由，一般家庭所用的路由为192.168.1.1）
子网掩码：255.255.255.0	   （这里基本都是一样的）
物理地址MAC：0C.29.AB.7C.00.02         (必须保证第一个字节为偶数，其他字节数据值随便)
本机IP（W5500模块IP）：192.168.0.246   （只要和目标IP不冲突即可）
本机端口：5000  （一般默认）
目标IP：192.168.0.149	    （和模块要在同一网关下）
目标端口：6000  （一般默认）

*/


#include "main_w5500.h"


//u32 W5500_Send_Delay=0; //W5500发送延时计数变量(ms)

//void setSocketinfo(unsigned char *pdata , char *dat , int len , char *zzz)
//{
//	int i =0 ;
//	char *temp;
//	char buff[50];
//	strcpy((char *)buff,dat);
//	temp = strtok(buff,zzz);
//	pdata[0]=atoi(temp);
//	for(i = 1 ;i < len ;i ++)
//	{		
//		temp = strtok(NULL,zzz);
//		pdata[i]=atoi(temp);	
//	}
//}



//装载网络参数
//说明: 网关、子网掩码、物理地址、本机IP地址、本机端口号、目的IP地址、目的端口号、端口工作模式







//void getW5500Data(SOCKET socket)
//{
//		//printf("wxc\r\n");
//		
//		//if(W5500_Interrupt)//处理W5500中断		
//	//	{
//			//W5500_Interrupt_Process();//W5500中断处理程序框架
//		//}
//		//if((S0_Data & S_RECEIVE) == S_RECEIVE)//如果Socket0接收到数据
//		{
//		//	S0_Data&=~S_RECEIVE;
//			//Process_Socket_Data(socket);//W5500接收并发送接收到的数据
//		}		
//	//	else 
//		if(W5500_Send_Delay>=10)//定时发送字符串
//		{
//			if(socketInfo[socket].State== (S_INIT|S_CONN))
//			{
//				socketInfo[socket].DataFlag&=~S_TRANSMITOK;
//				//memcpy(Tx_Buffer, "\r\n启光科技 W5500客户端TEST\r\n", 30);	
//				//Write_SOCK_Data_Buffer(0, Tx_Buffer, 30);//指定Socket(0~7)发送数据处理,端口0发送30字节数据
//			}
//			W5500_Send_Delay=0;
//		}	
//		W5500_Send_Delay++;
//}






