

/*
 http://zq281598585.taobao.com/  
�������	  ELH    enlighten  sunny

w5500����ģ��  �ο�����

���в���MCU STM32F103RBT6


TCP�ͻ���ģʽ 

�ж��жϷ�ʽ
TCP�ͻ����ǽ�ģ���Լ�����Ϊ�ͻ���Ȼ��ͷ������˽�������ͨѶ
��˴�ģʽҪȷ��ģ���Լ���IP���˿ں�Ŀ���������IP���˿�


W5500�������Ŷ�Ӧ���£� 
sck PA5  miso PA6  mosi PA7  rst PA2  int PA3  cs PA0



����·�ɸ�����ֵ

���أ�192.168.0.1		   ����Ϊ���ǲ���·�ɣ�һ���ͥ���õ�·��Ϊ192.168.1.1��
�������룺255.255.255.0	   �������������һ���ģ�
�����ַMAC��0C.29.AB.7C.00.02         (���뱣֤��һ���ֽ�Ϊż���������ֽ�����ֵ���)
����IP��W5500ģ��IP����192.168.0.246   ��ֻҪ��Ŀ��IP����ͻ���ɣ�
�����˿ڣ�5000  ��һ��Ĭ�ϣ�
Ŀ��IP��192.168.0.149	    ����ģ��Ҫ��ͬһ�����£�
Ŀ��˿ڣ�6000  ��һ��Ĭ�ϣ�

*/


#include "main_w5500.h"


//u32 W5500_Send_Delay=0; //W5500������ʱ��������(ms)

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



//װ���������
//˵��: ���ء��������롢�����ַ������IP��ַ�������˿ںš�Ŀ��IP��ַ��Ŀ�Ķ˿ںš��˿ڹ���ģʽ







//void getW5500Data(SOCKET socket)
//{
//		//printf("wxc\r\n");
//		
//		//if(W5500_Interrupt)//����W5500�ж�		
//	//	{
//			//W5500_Interrupt_Process();//W5500�жϴ��������
//		//}
//		//if((S0_Data & S_RECEIVE) == S_RECEIVE)//���Socket0���յ�����
//		{
//		//	S0_Data&=~S_RECEIVE;
//			//Process_Socket_Data(socket);//W5500���ղ����ͽ��յ�������
//		}		
//	//	else 
//		if(W5500_Send_Delay>=10)//��ʱ�����ַ���
//		{
//			if(socketInfo[socket].State== (S_INIT|S_CONN))
//			{
//				socketInfo[socket].DataFlag&=~S_TRANSMITOK;
//				//memcpy(Tx_Buffer, "\r\n����Ƽ� W5500�ͻ���TEST\r\n", 30);	
//				//Write_SOCK_Data_Buffer(0, Tx_Buffer, 30);//ָ��Socket(0~7)�������ݴ���,�˿�0����30�ֽ�����
//			}
//			W5500_Send_Delay=0;
//		}	
//		W5500_Send_Delay++;
//}






