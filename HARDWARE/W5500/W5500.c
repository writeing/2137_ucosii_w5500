/**********************************************************************************
W5500����ģ�� ������

**********************************************************************************/

#include "stm32f10x.h"
#include "spi.h"				
#include "W5500.h"
#include "delay.h"	


//��������������� 
socketlocalinfo socketLocalInfo;
socketinfo socketInfo[4];
unsigned char UDP_DIPR[4];	    //UDP(�㲥)ģʽ,Ŀ������IP��ַ
unsigned char UDP_DPORT[2];	//UDP(�㲥)ģʽ,Ŀ�������˿ں�

char targetIP[20];


unsigned char W5500_Interrupt;	//W5500�жϱ�־(0:���ж�,1:���ж�)


int recv(int socket)
{
	if(Read_SOCK_Data_Buffer(socket,socketInfo[socket].Rx_Buffer))
	{												
		return strlen((char *)socketInfo[socket].Rx_Buffer);
	}			
	return 0;
}

char *gettargetIP(SOCKET s)
{
//	sprintf(targetIP,"%d.%d.%d.%d",socketInfo[s].DIP[0],socketInfo[s].DIP[1],socketInfo[s].DIP[2],socketInfo[s].DIP[3]);
	sprintf(targetIP,"%d.%d.%d.%d",socketLocalInfo.IP_Addr[0],\
	socketLocalInfo.IP_Addr[1],socketLocalInfo.IP_Addr[2],socketLocalInfo.IP_Addr[3]);
	printf("%s\r\n",targetIP);
	sprintf(targetIP,"%d.%d.%d.%d",socketInfo[s].DPort[0],\
	socketInfo[s].DPort[1],socketInfo[s].Port[0],socketInfo[s].Port[1]);
	
	return targetIP;
}


//W5500���˿ڳ�ʼ���������ж�ģʽ
//��Ӧ����˵����
//sck PA5  miso PA6  mosi PA7  rst PA2  int PA3  cs PA0
void W5500_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;	

// 	EXTI_InitTypeDef EXTI_InitStructure;			//�ж�����	  ���ĸ��ж���  EXTI_Line0-15
													//ģʽ EXTI_Mode_Interrupt�ж�  EXTI_Mode_Event �¼�
													//������ʽ  EXTI_Trigger_Falling �½��ش���
													//			EXTI_Trigger_Rising	 �����ش���
													//			EXTI_Trigger_Rising_Falling	  �����ƽ����

// 	NVIC_InitTypeDef NVIC_InitStructure;			//�жϲ��� �ж����ȼ�


  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��



	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );//PORTBʱ��ʹ�� 


	// W5500_RST���ų�ʼ������(PA2) 
	GPIO_InitStructure.GPIO_Pin  = W5500_RST;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(W5500_RST_GPIO, &GPIO_InitStructure);
	GPIO_ResetBits(W5500_RST_GPIO, W5500_RST);
	
//	// W5500_INT���ų�ʼ������(PA3) 	
//	GPIO_InitStructure.GPIO_Pin = W5500_INT;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_Init(W5500_INT_GPIO, &GPIO_InitStructure);


	// ��ʼ������ģ��SPI-CS���� (PA0)
	GPIO_InitStructure.GPIO_Pin = W5500_SCS;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(W5500_SCS_GPIO, &GPIO_InitStructure);
	GPIO_SetBits(W5500_SCS_GPIO, W5500_SCS);



// 	// Enable the EXTI3 Interrupt 
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;				//W5500_INT���ڵ��ⲿ�ж�ͨ��
//  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;			//�����ȼ�2
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ⲿ�ж�ͨ��
//	NVIC_Init(&NVIC_InitStructure);
//
//
//  	// Connect EXTI Line3 to PA3 
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);
//
//
//	// PA3 as W5500 interrupt input 
//	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);

}


////�ж���3  PA3��Ӧ W5500�������� ����һ��־λ
//void EXTI3_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
//	{
//		EXTI_ClearITPendingBit(EXTI_Line3);	//���ж���
//		W5500_Interrupt=1;
//	}
//}



//ͨ��SPI1����һ���ֽ�
//dat ���͵��ֽ�
//�޷���
void SPI1_Send_Byte(unsigned char dat)
{
	SPI_I2S_SendData(SPI1,dat);//д1���ֽ�����
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);//��ѭ���ȴ����ݼĴ�����
}



//PI1����2���ֽ�����(16λ)
//dat:�����͵�16λ����
void SPI1_Send_Short(unsigned short dat)
{
	SPI1_Send_Byte(dat/256);//д���ݸ�λ   �൱��dat>>8
	SPI1_Send_Byte(dat);	//д���ݵ�λ
}



//ͨ��SPI1��W5500ָ����ַ�Ĵ���д1���ֽ�����
// reg:16λ�Ĵ�����ַ,dat:��д�������
void Write_W5500_1Byte(unsigned short reg, unsigned char dat)
{
	GPIO_ResetBits(W5500_SCS_GPIO, W5500_SCS);//CSƬѡW5500

	SPI1_Send_Short(reg);   //ͨ��SPI1д16λ�Ĵ�����ַ
	SPI1_Send_Byte(FDM1|RWB_WRITE|COMMON_R); //ͨ��SPI1д�����ֽ�,1���ֽ����ݳ���,д����,ѡ��ͨ�üĴ���
	SPI1_Send_Byte(dat);  //д1���ֽ�����

	GPIO_SetBits(W5500_SCS_GPIO, W5500_SCS); //����CS  ȡ��Ƭѡ
}




//ͨ��SPI1��ָ����ַ�Ĵ���д2���ֽ�����
//reg:16λ�Ĵ�����ַ,dat:16λ��д�������(2���ֽ�)
void Write_W5500_2Byte(unsigned short reg, unsigned short dat)
{
	GPIO_ResetBits(W5500_SCS_GPIO, W5500_SCS);//CSƬѡW5500
		
	SPI1_Send_Short(reg);//ͨ��SPI1д16λ�Ĵ�����ַ
	SPI1_Send_Byte(FDM2|RWB_WRITE|COMMON_R);//ͨ��SPI1д�����ֽ�,2���ֽ����ݳ���,д����,ѡ��ͨ�üĴ���
	SPI1_Send_Short(dat);//д16λ����

	GPIO_SetBits(W5500_SCS_GPIO, W5500_SCS); //����CS  ȡ��Ƭѡ
}




//ͨ��SPI1��ָ����ַ�Ĵ���дn���ֽ�����
//reg:16λ�Ĵ�����ַ,*dat_ptr:��д�����ݻ�����ָ��,size:��д������ݳ���
void Write_W5500_nByte(unsigned short reg, unsigned char *dat_ptr, unsigned short size)
{
	unsigned short i;

	GPIO_ResetBits(W5500_SCS_GPIO, W5500_SCS);//CSƬѡW5500	
		
	SPI1_Send_Short(reg);//ͨ��SPI1д16λ�Ĵ�����ַ
	SPI1_Send_Byte(VDM|RWB_WRITE|COMMON_R);//ͨ��SPI1д�����ֽ�,N���ֽ����ݳ���,д����,ѡ��ͨ�üĴ���

	for(i=0;i<size;i++)//ѭ������������size���ֽ�����д��W5500
	{
		SPI1_Send_Byte(*dat_ptr++);//дһ���ֽ�����
	}

	GPIO_SetBits(W5500_SCS_GPIO, W5500_SCS); //����CS  ȡ��Ƭѡ
}




//ͨ��SPI1��ָ���˿ڼĴ���д1���ֽ�����
//s:�˿ں�,reg:16λ�Ĵ�����ַ,dat:��д�������
void Write_W5500_SOCK_1Byte(SOCKET s, unsigned short reg, unsigned char dat)
{
	GPIO_ResetBits(W5500_SCS_GPIO, W5500_SCS);//CSƬѡW5500	
		
	SPI1_Send_Short(reg);//ͨ��SPI1д16λ�Ĵ�����ַ
	SPI1_Send_Byte(FDM1|RWB_WRITE|(s*0x20+0x08));//ͨ��SPI1д�����ֽ�,1���ֽ����ݳ���,д����,ѡ��˿�s�ļĴ���
	SPI1_Send_Byte(dat);//д1���ֽ�����

	GPIO_SetBits(W5500_SCS_GPIO, W5500_SCS); //����CS  ȡ��Ƭѡ
}



//ͨ��SPI1��ָ���˿ڼĴ���д2���ֽ�����
//s:�˿ں�,reg:16λ�Ĵ�����ַ,dat:16λ��д�������(2���ֽ�)
void Write_W5500_SOCK_2Byte(SOCKET s, unsigned short reg, unsigned short dat)
{
	GPIO_ResetBits(W5500_SCS_GPIO, W5500_SCS);//CSƬѡW5500
			
	SPI1_Send_Short(reg);//ͨ��SPI1д16λ�Ĵ�����ַ
	SPI1_Send_Byte(FDM2|RWB_WRITE|(s*0x20+0x08));//ͨ��SPI1д�����ֽ�,2���ֽ����ݳ���,д����,ѡ��˿�s�ļĴ���
	SPI1_Send_Short(dat);//д16λ����

	GPIO_SetBits(W5500_SCS_GPIO, W5500_SCS); //����CS  ȡ��Ƭѡ
}




//ͨ��SPI1��ָ���˿ڼĴ���д4���ֽ�����
//s:�˿ں�,reg:16λ�Ĵ�����ַ,*dat_ptr:��д���4���ֽڻ�����ָ��
void Write_W5500_SOCK_4Byte(SOCKET s, unsigned short reg, unsigned char *dat_ptr)
{
	GPIO_ResetBits(W5500_SCS_GPIO, W5500_SCS);//����CS CSƬѡW5500
			
	SPI1_Send_Short(reg);//ͨ��SPI1д16λ�Ĵ�����ַ
	SPI1_Send_Byte(FDM4|RWB_WRITE|(s*0x20+0x08));//ͨ��SPI1д�����ֽ�,4���ֽ����ݳ���,д����,ѡ��˿�s�ļĴ���

	SPI1_Send_Byte(*dat_ptr++);//д��1���ֽ�����
	SPI1_Send_Byte(*dat_ptr++);//д��2���ֽ�����
	SPI1_Send_Byte(*dat_ptr++);//д��3���ֽ�����
	SPI1_Send_Byte(*dat_ptr++);//д��4���ֽ�����

	GPIO_SetBits(W5500_SCS_GPIO, W5500_SCS);//����CS  ȡ��Ƭѡ
}



//��ȡW5500ָ����ַ�Ĵ�����1���ֽ�����
//reg:16λ�Ĵ�����ַ
//����:��ȡ���Ĵ�����1���ֽ�����
unsigned char Read_W5500_1Byte(unsigned short reg)
{
	unsigned char i;

	GPIO_ResetBits(W5500_SCS_GPIO, W5500_SCS);//����CS CSƬѡW5500
			
	SPI1_Send_Short(reg);  //ͨ��SPI1д16λ�Ĵ�����ַ
	SPI1_Send_Byte(FDM1|RWB_READ|COMMON_R);//ͨ��SPI1д�����ֽ�,1���ֽ����ݳ���,������,ѡ��ͨ�üĴ���

	i=SPI_I2S_ReceiveData(SPI1);
	SPI1_Send_Byte(0x00);//���Ϳ����� �ȴ���������
	i=SPI_I2S_ReceiveData(SPI1);//��ȡ1���ֽ�����

	GPIO_SetBits(W5500_SCS_GPIO, W5500_SCS);//����CS  ȡ��Ƭѡ
	return i;//���ض�ȡ���ļĴ�������
}



//��W5500ָ���˿ڼĴ�����1���ֽ�����
//s:�˿ں�,reg:16λ�Ĵ�����ַ
//����ֵ:��ȡ���Ĵ�����1���ֽ�����
unsigned char Read_W5500_SOCK_1Byte(SOCKET s, unsigned short reg)
{
	unsigned char i;

	GPIO_ResetBits(W5500_SCS_GPIO, W5500_SCS);//����CS CSƬѡW5500
			
	SPI1_Send_Short(reg);   //ͨ��SPI1д16λ�Ĵ�����ַ
	SPI1_Send_Byte(FDM1|RWB_READ|(s*0x20+0x08));//ͨ��SPI1д�����ֽ�,1���ֽ����ݳ���,������,ѡ��˿�s�ļĴ���

	i=SPI_I2S_ReceiveData(SPI1);
	SPI1_Send_Byte(0x00);   //���Ϳ����� �ȴ���������
	i=SPI_I2S_ReceiveData(SPI1);//��ȡ1���ֽ�����

	GPIO_SetBits(W5500_SCS_GPIO, W5500_SCS);  //����CS  ȡ��Ƭѡ
	return i;//���ض�ȡ���ļĴ�������
}



//��W5500ָ���˿ڼĴ�����2���ֽ�����
//s:�˿ں�,reg:16λ�Ĵ�����ַ
//����ֵ:��ȡ���Ĵ�����2���ֽ�����(16λ)
unsigned short Read_W5500_SOCK_2Byte(SOCKET s, unsigned short reg)
{
	unsigned short i;

	GPIO_ResetBits(W5500_SCS_GPIO, W5500_SCS);//����CS CSƬѡW5500
			
	SPI1_Send_Short(reg);//ͨ��SPI1д16λ�Ĵ�����ַ
	SPI1_Send_Byte(FDM2|RWB_READ|(s*0x20+0x08));//ͨ��SPI1д�����ֽ�,2���ֽ����ݳ���,������,ѡ��˿�s�ļĴ���

	i=SPI_I2S_ReceiveData(SPI1);
	SPI1_Send_Byte(0x00);//���Ϳ����� �ȴ���������
	i=SPI_I2S_ReceiveData(SPI1);//��ȡ��λ����
	SPI1_Send_Byte(0x00);//���Ϳ����� �ȴ���������
	i*=256;
	i+=SPI_I2S_ReceiveData(SPI1);//��ȡ��λ����

	GPIO_SetBits(W5500_SCS_GPIO, W5500_SCS);//����CS  ȡ��Ƭѡ
	return i;//���ض�ȡ���ļĴ�������
}




//��W5500�������ݻ������ж�ȡ����
//s:�˿ں�,*dat_ptr:���ݱ��滺����ָ��
//����ֵ:��ȡ�������ݳ���,rx_size���ֽ�
unsigned short Read_SOCK_Data_Buffer(SOCKET s, unsigned char *dat_ptr)
{
	unsigned short rx_size;
	unsigned short offset, offset1;
	unsigned short i;
	unsigned char j;

	rx_size=Read_W5500_SOCK_2Byte(s,Sn_RX_RSR);
	if(rx_size==0) return 0;  //û���յ������򷵻�
	if(rx_size>1460) rx_size=1460;

	offset=Read_W5500_SOCK_2Byte(s,Sn_RX_RD);
	offset1=offset;
	offset&=(S_RX_SIZE-1);    //����ʵ�ʵ������ַ

	GPIO_ResetBits(W5500_SCS_GPIO, W5500_SCS);//����CS CSƬѡW5500

	SPI1_Send_Short(offset);//д16λ��ַ
	SPI1_Send_Byte(VDM|RWB_READ|(s*0x20+0x18));//д�����ֽ�,N���ֽ����ݳ���,������,ѡ��˿�s�ļĴ���
	j=SPI_I2S_ReceiveData(SPI1);
	
	if((offset+rx_size)<S_RX_SIZE)//�������ַδ����W5500���ջ������Ĵ���������ַ
	{
		for(i=0;i<rx_size;i++)//ѭ����ȡrx_size���ֽ�����
		{
			SPI1_Send_Byte(0x00);//����һ��������
			j=SPI_I2S_ReceiveData(SPI1);//��ȡ1���ֽ�����
			*dat_ptr=j;//����ȡ�������ݱ��浽���ݱ��滺����
			dat_ptr++;//���ݱ��滺����ָ���ַ����1
		}
	}
	else//�������ַ����W5500���ջ������Ĵ���������ַ
	{
		offset=S_RX_SIZE-offset;
		for(i=0;i<offset;i++)//ѭ����ȡ��ǰoffset���ֽ�����
		{
			SPI1_Send_Byte(0x00);//����һ��������
			j=SPI_I2S_ReceiveData(SPI1);//��ȡ1���ֽ�����
			*dat_ptr=j;//����ȡ�������ݱ��浽���ݱ��滺����
			dat_ptr++;//���ݱ��滺����ָ���ַ����1
		}
		GPIO_SetBits(W5500_SCS_GPIO, W5500_SCS);  //����CS  ȡ��Ƭѡ

		GPIO_ResetBits(W5500_SCS_GPIO, W5500_SCS);//����CS CSƬѡW5500

		SPI1_Send_Short(0x00);//д16λ��ַ
		SPI1_Send_Byte(VDM|RWB_READ|(s*0x20+0x18));//д�����ֽ�,N���ֽ����ݳ���,������,ѡ��˿�s�ļĴ���
		j=SPI_I2S_ReceiveData(SPI1);

		for(;i<rx_size;i++)//ѭ����ȡ��rx_size-offset���ֽ�����
		{
			SPI1_Send_Byte(0x00);//����һ��������
			j=SPI_I2S_ReceiveData(SPI1);//��ȡ1���ֽ�����
			*dat_ptr=j;//����ȡ�������ݱ��浽���ݱ��滺����
			dat_ptr++;//���ݱ��滺����ָ���ַ����1
		}
	}
	GPIO_SetBits(W5500_SCS_GPIO, W5500_SCS); //����CS  ȡ��Ƭѡ

	offset1+=rx_size;//����ʵ�������ַ,���´ζ�ȡ���յ������ݵ���ʼ��ַ
	Write_W5500_SOCK_2Byte(s, Sn_RX_RD, offset1);
	Write_W5500_SOCK_1Byte(s, Sn_CR, RECV);//����������������
	return rx_size;//���ؽ��յ����ݵĳ���
}



//������д��W5500�����ݷ��ͻ�����
//s:�˿ں�,*dat_ptr:���ݱ��滺����ָ��,size:��д�����ݵĳ���
void Write_SOCK_Data_Buffer(SOCKET s, unsigned char *dat_ptr, unsigned short size)
{
	unsigned short offset,offset1;
	unsigned short i;

	//�����UDPģʽ,�����ڴ�����Ŀ��������IP�Ͷ˿ں�
	if((Read_W5500_SOCK_1Byte(s,Sn_MR)&0x0f) != SOCK_UDP)//���Socket��ʧ��
	{		
		Write_W5500_SOCK_4Byte(s, Sn_DIPR, UDP_DIPR);//����Ŀ������IP  		
		Write_W5500_SOCK_2Byte(s, Sn_DPORTR, UDP_DPORT[0]*256+UDP_DPORT[1]);//����Ŀ�������˿ں�				
	}

	offset=Read_W5500_SOCK_2Byte(s,Sn_TX_WR);
	offset1=offset;
	offset&=(S_TX_SIZE-1);//����ʵ�ʵ������ַ

	GPIO_ResetBits(W5500_SCS_GPIO, W5500_SCS);//��W5500��SCSΪ�͵�ƽ

	SPI1_Send_Short(offset);//д16λ��ַ
	SPI1_Send_Byte(VDM|RWB_WRITE|(s*0x20+0x10));//д�����ֽ�,N���ֽ����ݳ���,д����,ѡ��˿�s�ļĴ���

	if((offset+size)<S_TX_SIZE)//�������ַδ����W5500���ͻ������Ĵ���������ַ
	{
		for(i=0;i<size;i++)//ѭ��д��size���ֽ�����
		{
			SPI1_Send_Byte(*dat_ptr++);//д��һ���ֽڵ�����		
		}
	}
	else//�������ַ����W5500���ͻ������Ĵ���������ַ
	{
		offset=S_TX_SIZE-offset;
		for(i=0;i<offset;i++)//ѭ��д��ǰoffset���ֽ�����
		{
			SPI1_Send_Byte(*dat_ptr++);//д��һ���ֽڵ�����
		}
		GPIO_SetBits(W5500_SCS_GPIO, W5500_SCS); //��W5500��SCSΪ�ߵ�ƽ

		GPIO_ResetBits(W5500_SCS_GPIO, W5500_SCS);//��W5500��SCSΪ�͵�ƽ

		SPI1_Send_Short(0x00);//д16λ��ַ
		SPI1_Send_Byte(VDM|RWB_WRITE|(s*0x20+0x10));//д�����ֽ�,N���ֽ����ݳ���,д����,ѡ��˿�s�ļĴ���

		for(;i<size;i++)//ѭ��д��size-offset���ֽ�����
		{
			SPI1_Send_Byte(*dat_ptr++);//д��һ���ֽڵ�����
		}
	}
	GPIO_SetBits(W5500_SCS_GPIO, W5500_SCS); //��W5500��SCSΪ�ߵ�ƽ

	offset1+=size;//����ʵ�������ַ,���´�д���������ݵ��������ݻ���������ʼ��ַ
	Write_W5500_SOCK_2Byte(s, Sn_TX_WR, offset1);
	Write_W5500_SOCK_1Byte(s, Sn_CR, SEND);//����������������				
}


//W5500Ӳ����λ
//˵����W5500�ĸ�λ���ű��ֵ͵�ƽ����500us����,����ʹW5500���븴λ״̬
void W5500_Hardware_Reset(void)
{
	GPIO_ResetBits(W5500_RST_GPIO, W5500_RST);  //��λ��������
	delay_ms(50);
	GPIO_SetBits(W5500_RST_GPIO, W5500_RST);    //��λ��������
	delay_ms(200);
	while((Read_W5500_1Byte(PHYCFGR)&LINK)==0); //��ѭ���ȴ���̫���������	����Ҫ���������ӵ�·�ɺ�ģ���� �����޷�ͨ��
}




//��ʼ��W5500��Ӧ�Ĵ���
//˵����Ӳ����ʼ����Ҫ���������Ӧ�����ʼ��
void W5500_Init(void)
{
	unsigned char i=0;

	Write_W5500_1Byte(MR, RST);  //�����λW5500,��1��Ч,��λ���Զ���0
	delay_ms(10);                //��ʱ10ms

	//��������(Gateway)��IP��ַ,Gateway_IPΪ4�ֽ�
	//ʹ�����ؿ���ʹͨ��ͻ�������ľ��ޣ�ͨ�����ؿ��Է��ʵ��������������Internet
	Write_W5500_nByte(GAR, socketLocalInfo.Gateway_IP, 4);
			
	//������������(MASK)ֵ,SUB_MASKΪ4�ֽ�
	//��������������������
	Write_W5500_nByte(SUBR,socketLocalInfo.Sub_Mask,4);		
	
	//���������ַ,PHY_ADDRΪ6�ֽ�,����Ψһ��ʶ�����豸�������ֵַ
	//�õ�ֵַ��Ҫ��IEEE���룬����OUI�Ĺ涨��ǰ3���ֽ�Ϊ���̴��룬�������ֽ�Ϊ��Ʒ���
	//����Լ����������ַ��ע���һ���ֽڱ���Ϊż��
	Write_W5500_nByte(SHAR,socketLocalInfo.Phy_Addr,6);		

	//���ñ�����IP��ַ,IP_ADDRΪ4�ֽ�
	//ע�⣬����IP�����뱾��IP����ͬһ�����������򱾻����޷��ҵ�����
	Write_W5500_nByte(SIPR,socketLocalInfo.IP_Addr,4);		
	
	//���÷��ͻ������ͽ��ջ������Ĵ�С���ο�W5500�����ֲ� ��ʼ���ֱ�����Ϊ2k �ܹ�Ϊ16k
	for(i=0;i<8;i++)
	{
		Write_W5500_SOCK_1Byte(i,Sn_RXBUF_SIZE, 0x02);//Socket Rx memory size=2k
		Write_W5500_SOCK_1Byte(i,Sn_TXBUF_SIZE, 0x02);//Socket Tx mempry size=2k
	}

	//��������ʱ�䣬Ĭ��Ϊ2000(200ms) 
	//ÿһ��λ��ֵΪ100΢��,��ʼ��ʱֵ��Ϊ2000(0x07D0),����200����
	Write_W5500_2Byte(RTR, 0x07d0);

	//�������Դ�����Ĭ��Ϊ8�� 
	//����ط��Ĵ��������趨ֵ,�������ʱ�ж�(��صĶ˿��жϼĴ����е�Sn_IR ��ʱλ(TIMEOUT)�á�1��)
	Write_W5500_1Byte(RCR_WXC,8);

//	//�����жϣ��ο�W5500�����ֲ�ȷ���Լ���Ҫ���ж�����
//	//IMR_CONFLICT��IP��ַ��ͻ�쳣�ж�,IMR_UNREACH��UDPͨ��ʱ����ַ�޷�������쳣�ж�
//	//������Socket�¼��жϣ�������Ҫ���
//	Write_W5500_1Byte(IMR,IM_IR7 | IM_IR6);	//�ж�����Ĵ���
//	Write_W5500_1Byte(SIMR,S0_IMR);			//socket�ж�״̬0-7 �Ĵ���������S0��
//	Write_W5500_SOCK_1Byte(0, Sn_IMR, IMR_SENDOK | IMR_TIMEOUT | IMR_RECV | IMR_DISCON | IMR_CON);	//S0 �жϹ���ȫ����
}




//������ط�����
//����ģ��һ�˿ڵ���Ӧ�������ж�W5500ģʽ�Ƿ�������ͨѶ״̬��
//����ֵ:�ɹ�����TRUE(0xFF),ʧ�ܷ���FALSE(0x00)
unsigned char Detect_Gateway(SOCKET s)
{
	unsigned char ip_adde[4];
	ip_adde[0]=socketLocalInfo.IP_Addr[0]+1;
	ip_adde[1]=socketLocalInfo.IP_Addr[1]+1;
	ip_adde[2]=socketLocalInfo.IP_Addr[2]+1;
	ip_adde[3]=socketLocalInfo.IP_Addr[3]+1;

	//������ؼ���ȡ���ص������ַ
	Write_W5500_SOCK_4Byte(s,Sn_DIPR,ip_adde);//��Ŀ�ĵ�ַ�Ĵ���д���뱾��IP��ͬ��IPֵ
	Write_W5500_SOCK_1Byte(s,Sn_MR,MR_TCP);   //����socketΪTCPģʽ
	Write_W5500_SOCK_1Byte(s,Sn_CR,OPEN);     //��Socket	
	delay_ms(5);//��ʱ5ms 	
	
	if(Read_W5500_SOCK_1Byte(s,Sn_SR) != SOCK_INIT)//���socket��ʧ��	 ������TCPģʽʧ��
	{
		Write_W5500_SOCK_1Byte(s,Sn_CR,CLOSE);//�򿪲��ɹ�,�ر�Socket
		return FALSE;//����FALSE(0x00)
	}

	Write_W5500_SOCK_1Byte(s,Sn_CR,CONNECT);//����SocketΪConnectģʽ	TCP��Ϊ�ͻ���					

	do
	{
		unsigned char j=0;
		j=Read_W5500_SOCK_1Byte(s,Sn_IR);//��ȡSocket0�жϱ�־�Ĵ���
		if(j!=0)						 //������ж���ʾ ��д1���Ӧ�ж�
		Write_W5500_SOCK_1Byte(s,Sn_IR,j);
		delay_ms(5);//��ʱ5ms 
		if((j&IR_TIMEOUT) == IR_TIMEOUT) //��ʱ
		{
			return FALSE;	
		}
		else if(Read_W5500_SOCK_1Byte(s,Sn_DHAR) != 0xff)	 //ֻ�ж�ĩλMAC �Ƿ�ΪFF ����ɹ���������Ӧ��ΪFF
		{
			Write_W5500_SOCK_1Byte(s,Sn_CR,CLOSE);//�ر�Socket	   ˵�����Գɹ�
			return TRUE;							
		}
	}while(1);
}





//ָ��Socket(0~7)��ʼ��
//s:����ʼ���Ķ˿�
void Socket_Init(SOCKET s)
{
	//���÷�Ƭ���ȣ��ο�W5500�����ֲᣬ��ֵ���Բ��޸�	
	Write_W5500_SOCK_2Byte(s, Sn_MSSR, 1460);//����Ƭ�ֽ���=1460(0x5b4)
	//����ָ���˿�
	//���ö˿�0�Ķ˿ں�
	Write_W5500_SOCK_2Byte(s, Sn_PORT, socketInfo[s].Port[0]*256+socketInfo[s].Port[1]);
	//���ö˿�0Ŀ��(Զ��)�˿ں�
	Write_W5500_SOCK_2Byte(s, Sn_DPORTR, socketInfo[s].DPort[0]*256+socketInfo[s].DPort[1]);
	//���ö˿�0Ŀ��(Զ��)IP��ַ
	Write_W5500_SOCK_4Byte(s, Sn_DIPR, socketInfo[s].DIP);			

}



//����ָ��Socket(0~7)Ϊ�ͻ�����Զ�̷���������
//s:���趨�Ķ˿�
//����ֵ:�ɹ�����TRUE(0xFF),ʧ�ܷ���FALSE(0x00)
//˵��  :������Socket�����ڿͻ���ģʽʱ,���øó���,��Զ�̷�������������
//	     ����������Ӻ���ֳ�ʱ�жϣ��������������ʧ��,��Ҫ���µ��øó�������
//	     �ó���ÿ����һ��,�������������һ������
unsigned char Socket_Connect(SOCKET s)
{
	Write_W5500_SOCK_1Byte(s,Sn_MR,MR_TCP);//����socketΪTCPģʽ
	Write_W5500_SOCK_1Byte(s,Sn_CR,OPEN);//��Socket
	delay_ms(5);//��ʱ5ms
	if(Read_W5500_SOCK_1Byte(s,Sn_SR)!=SOCK_INIT)//���socket��ʧ��
	{
		Write_W5500_SOCK_1Byte(s,Sn_CR,CLOSE);//�򿪲��ɹ�,�ر�Socket
		return FALSE;//����FALSE(0x00)
	}
	Write_W5500_SOCK_1Byte(s,Sn_CR,CONNECT);//����SocketΪConnectģʽ
	return TRUE;//����TRUE,���óɹ�
}

//����ָ��Socket(0~7)��Ϊ�������ȴ�Զ������������
//s:���趨�Ķ˿�
//����ֵ:�ɹ�����TRUE(0xFF),ʧ�ܷ���FALSE(0x00)
//˵��  :������Socket�����ڷ�����ģʽʱ,���øó���,�ȵ�Զ������������
//		 �ó���ֻ����һ��,��ʹW5500����Ϊ������ģʽ
unsigned char Socket_Listen(SOCKET s)
{
	Write_W5500_SOCK_1Byte(s,Sn_MR,MR_TCP);//����socketΪTCPģʽ 
	Write_W5500_SOCK_1Byte(s,Sn_CR,OPEN);//��Socket	
	delay_ms(5);//��ʱ5ms
	if(Read_W5500_SOCK_1Byte(s,Sn_SR)!=SOCK_INIT)//���socket��ʧ��
	{
		Write_W5500_SOCK_1Byte(s,Sn_CR,CLOSE);//�򿪲��ɹ�,�ر�Socket
		return FALSE;//����FALSE(0x00)
	}	
	Write_W5500_SOCK_1Byte(s,Sn_CR,LISTEN);//����SocketΪ����ģʽ	
	delay_ms(5);//��ʱ5ms
	if(Read_W5500_SOCK_1Byte(s,Sn_SR)!=SOCK_LISTEN)//���socket����ʧ��
	{
		Write_W5500_SOCK_1Byte(s,Sn_CR,CLOSE);//���ò��ɹ�,�ر�Socket
		return FALSE;//����FALSE(0x00)
	}

	return TRUE;

	//���������Socket�Ĵ򿪺�������������,����Զ�̿ͻ����Ƿ�������������,����Ҫ�ȴ�Socket�жϣ�
	//���ж�Socket�������Ƿ�ɹ����ο�W5500�����ֲ��Socket�ж�״̬
	//�ڷ���������ģʽ����Ҫ����Ŀ��IP��Ŀ�Ķ˿ں�
}




//����ָ��Socket(0~7)ΪUDPģʽ
//s:���趨�Ķ˿�
//����ֵ:�ɹ�����TRUE(0xFF),ʧ�ܷ���FALSE(0x00)
//˵��  :���Socket������UDPģʽ,���øó���,��UDPģʽ��,Socketͨ�Ų���Ҫ��������
//		 �ó���ֻ����һ�Σ���ʹW5500����ΪUDPģʽ
unsigned char Socket_UDP(SOCKET s)
{
	Write_W5500_SOCK_1Byte(s,Sn_MR,MR_UDP);//����SocketΪUDPģʽ*/
	Write_W5500_SOCK_1Byte(s,Sn_CR,OPEN);//��Socket*/
	delay_ms(5);//��ʱ5ms
	if(Read_W5500_SOCK_1Byte(s,Sn_SR)!=SOCK_UDP)//���Socket��ʧ��
	{
		Write_W5500_SOCK_1Byte(s,Sn_CR,CLOSE);//�򿪲��ɹ�,�ر�Socket
		return FALSE;//����FALSE(0x00)
	}
	else
		return TRUE;

	//���������Socket�Ĵ򿪺�UDPģʽ����,������ģʽ��������Ҫ��Զ��������������
	//��ΪSocket����Ҫ��������,�����ڷ�������ǰ����������Ŀ������IP��Ŀ��Socket�Ķ˿ں�
	//���Ŀ������IP��Ŀ��Socket�Ķ˿ں��ǹ̶���,�����й�����û�иı�,��ôҲ��������������
}


void tcpCloseDataCon(SOCKET s)
{

	Write_W5500_SOCK_1Byte(s,Sn_CR,CLOSE);//�رն˿�,�ȴ����´����� 
	Socket_Init(s); 	//ָ��Socket(0~7)��ʼ��,��ʼ���˿�0
	socketInfo[s].State=0;//��������״̬0x00,�˿�����ʧ��
}


//������:W5500_Interrupt_Process
//����  :W5500�жϴ��������
void W5500_Interrupt_Process(void)
{
	unsigned char i,j;
	int index;
IntDispose:
//	W5500_Interrupt=0;//�����жϱ�־
//	i = Read_W5500_1Byte(IR);//��ȡ�жϱ�־�Ĵ���
//	Write_W5500_1Byte(IR, (i&0xf0));//��д����жϱ�־

//	if((i & CONFLICT) == CONFLICT)//IP��ַ��ͻ�쳣����
//	{
//		 //�Լ���Ӵ���
//	}
//
//	if((i & UNREACH) == UNREACH)//UDPģʽ�µ�ַ�޷������쳣����
//	{
//		//�Լ���Ӵ���
//	}

	i=Read_W5500_1Byte(SIR);//��ȡ�˿��жϱ�־�Ĵ���	
	for(index =0; index < 2 ; index ++)
	{
		//socketInfo[index].State
		//socketInfo[index].DataFlag

		if((i & (S0_INT << index)) == (S0_INT << index))//Socket0�¼����� 
		{
			j=Read_W5500_SOCK_1Byte(index,Sn_IR); //��ȡSocket0�жϱ�־�Ĵ���
			Write_W5500_SOCK_1Byte(index,Sn_IR,j);//��д����жϱ�־

			if(j&IR_CON)//��TCPģʽ��,Socket0�ɹ����� 
			{
				socketInfo[index].State|=S_CONN;//��������״̬0x02,�˿�������ӣ�����������������
			}
			if(j&IR_DISCON)//��TCPģʽ��Socket�Ͽ����Ӵ���
			{
				Write_W5500_SOCK_1Byte(index,Sn_CR,CLOSE);//�رն˿�,�ȴ����´����� 
				Socket_Init(index);		//ָ��Socket(0~7)��ʼ��,��ʼ���˿�0
				socketInfo[index].State=0;//��������״̬0x00,�˿�����ʧ��
			}
			if(j&IR_SEND_OK)//Socket0���ݷ������,�����ٴ�����S_tx_process()������������ 
			{
				socketInfo[index].DataFlag|=S_TRANSMITOK;//�˿ڷ���һ�����ݰ���� 
			}
			if(j&IR_RECV)//Socket���յ�����,��������S_rx_process()���� 
			{
				socketInfo[index].DataFlag|=S_RECEIVE;//�˿ڽ��յ�һ�����ݰ�
			}
			if(j&IR_TIMEOUT)//Socket���ӻ����ݴ��䳬ʱ���� 
			{
				Write_W5500_SOCK_1Byte(index,Sn_CR,CLOSE);// �رն˿�,�ȴ����´����� 
				socketInfo[index].State=0;//��������״̬0x00,�˿�����ʧ��
			}
		}
	}
	if(Read_W5500_1Byte(SIR) != 0) 
		goto IntDispose;
}

void setSocketinfo(unsigned char *pdata , char *dat , int len , char *zzz)
{
	int i =0 ;
	char *temp;
	char buff[50];
	strcpy((char *)buff,dat);
	temp = strtok(buff,zzz);
	pdata[0]=atoi(temp);
	for(i = 1 ;i < len ;i ++)
	{		
		temp = strtok(NULL,zzz);
		pdata[i]=atoi(temp);	
	}
	
}
void setTCPSocketinfo(SOCKET socket,int mode)
{	

	setSocketinfo(socketInfo[socket].Port,(char *)socketInfo[socket].info.Port,2,".");  //���ñ����˿�

	setSocketinfo(socketInfo[socket].DIP,	(char *)socketInfo[socket].info.DIP,4,".");	//����Ŀ��ip
	
	setSocketinfo(socketInfo[socket].DPort,	(char *)socketInfo[socket].info.DPort,2,".");				//����Ŀ�Ķ˿�
 	socketInfo[socket].Mode = mode; //TCP_SERVER TCP_CLIENT;			//��������ģʽ
 	socketInfo[socket].State = 0;			//��������״̬λ
}
void setUDPSocketinfo(SOCKET socket)
{
	setSocketinfo(socketInfo[socket].Port,(char *)socketInfo[socket].info.Port,2,".");  //���ñ����˿�

	setSocketinfo(socketInfo[socket].DIP,(char *)socketInfo[socket].info.DIP,4,".");	//����Ŀ��ip
	
	setSocketinfo(socketInfo[socket].DPort,(char *)socketInfo[socket].info.DPort,2,".");				//����Ŀ�Ķ˿�

 	socketInfo[socket].Mode = UDP_MODE;			//��������ģʽ
 	socketInfo[socket].State = 0;			//��������״̬λ
}
void setlocalip()
{
	int i = 0;
	for(i = 0;i < 3; i++)
	{
		socketLocalInfo.Gateway_IP[i]= socketInfo[0].DIP[i];

	}
	socketLocalInfo.Gateway_IP[i] = 1;

}

void Load_Net_Parameters(void)
{

//	setSocketinfo(socketLocalInfo.Gateway_IP,"169.254.77.1",4);

	setSocketinfo(socketLocalInfo.Sub_Mask,"255.255.255.0",4,".");

	setSocketinfo(socketLocalInfo.Phy_Addr,"12.41.171.124.0.2",6,".");

	setSocketinfo(socketLocalInfo.IP_Addr,(char *)iinfo.ip,4,".");
	
//	setSocketinfo(socketLocalInfo.IP_Addr,"192.168.77.66",4);

	setlocalip();
}

//����    : W5500�˿ڳ�ʼ������
//˵�� : �ֱ�����4���˿�,���ݶ˿ڹ���ģʽ,���˿�����TCP��������TCP�ͻ��˻�UDPģʽ.
//		 �Ӷ˿�״̬�ֽ�Socket_State�����ж϶˿ڵĹ������
void W5500_Socket_Set(SOCKET socket)
{
	if(socketInfo[socket].State==0)//�˿�0��ʼ������
	{
		if(socketInfo[socket].Mode==TCP_SERVER)//TCP������ģʽ 
		{
			if(Socket_Listen(socket)==TRUE)
				socketInfo[socket].State=S_INIT;
			else
				socketInfo[socket].State=0;
		}
		else if(socketInfo[socket].Mode==TCP_CLIENT)//TCP�ͻ���ģʽ 
		{
			if(Socket_Connect(socket)==TRUE)
				socketInfo[socket].State=S_INIT;
			else
				socketInfo[socket].State=0;
		}
		else//UDPģʽ 
		{
			if(Socket_UDP(socket)==TRUE)
				socketInfo[socket].State=S_INIT|S_CONN;
			else
				socketInfo[socket].State=0;
		}
	}
}
//W5500���ղ����ͽ��յ�������
//s:�˿ں�
//˵�� : �������ȵ���S_rx_process()��W5500�Ķ˿ڽ������ݻ�������ȡ����,
//		 Ȼ�󽫶�ȡ�����ݴ�Rx_Buffer������Temp_Buffer���������д���
//		 ������ϣ������ݴ�Temp_Buffer������Tx_Buffer������������S_tx_process()
//		 �������ݡ�
void Process_Socket_Data(SOCKET s)
{
	u16 size;
	
	u8 Open_Led[]="led open";
	u8 Close_Led[]="led close";
	size=Read_SOCK_Data_Buffer(s, socketInfo[s].Rx_Buffer);
													//���������string.hͷ�ļ�  strcmp���� 
													//ע��Ҫ�����溯�����޸� �������Ʋ�ͳһ ���Զ���Ϊu8
  if(strcmp((char *)socketInfo[s].Rx_Buffer,(char *)Open_Led)==0)led_pa1(1);	//����PA1  LED 

  if(strcmp((char *)socketInfo[s].Rx_Buffer,(char *)Close_Led)==0)led_pa1(0);	//�ر�PA1  LED  

	memcpy(socketInfo[s].Tx_Buffer, socketInfo[s].Rx_Buffer, size);	
			
	memset(socketInfo[s].Rx_Buffer, 0, size);						//�建����
	//Write_SOCK_Data_Buffer(s, Tx_Buffer, size);
	if(size > 0)
		printf("%s\r\n",socketInfo[s].Tx_Buffer);
	memset(socketInfo[s].Tx_Buffer, 0, size);						//�建����
}
//W5500��ʼ������
void W5500_Initialization(void)
{
	W5500_Init();		//��ʼ��W5500�Ĵ���
}


void init_w5500(void)
 {
	//delay_init();	    	 //��ʱ������ʼ��

	Load_Net_Parameters();		//װ���������	
	W5500_Hardware_Reset(); 	//Ӳ����λW5500
	W5500_Initialization(); 	//W5500��ʼ������

}

void W5500_connect(SOCKET s)
{
	Detect_Gateway(s);	//������ط����� 
	Socket_Init(s);		//ָ��Socket(0~7)��ʼ��,��ʼ���˿�0
	W5500_Socket_Set(s);  //connect
}

void initW5500_tcpSer(SOCKET s)
{
	strcpy((char *)iinfo.ip,(char *)"169.254.77.5");
	strcpy((char *)socketInfo[s].info.Port,(char *)"19.136");
	strcpy((char *)socketInfo[s].info.DIP,(char *)"169.254.77.79");
	strcpy((char *)socketInfo[s].info.DPort,(char *)"37.28");
}

void send(SOCKET s,u8 *data)
{
		Write_SOCK_Data_Buffer(s,data,strlen((char *)data));
}

