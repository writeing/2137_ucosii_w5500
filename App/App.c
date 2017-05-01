#include "stm32f10x.h"
#include "includes.h"

/*static */OS_STK startup_task_stk[STARTUP_TASK_STK_SIZE];
OS_STK led1_task_stk[LED1_TASK_STK_SIZE];
OS_STK show_task_stk[SHOW_TASK_STK_SIZE];
static void systick_init(void); //
static void recv_task(void *p_arg);
static void getCmd_task(void *p_arg);



int main(void)
{
	SystemCoreClockUpdate();
	OSInit();
	OSTaskCreate(getCmd_task, (void *)0,&startup_task_stk[STARTUP_TASK_STK_SIZE - 1],STARTUP_TASK_PRIO);
	OSTaskCreate(recv_task, (void *)0,&led1_task_stk[LED1_TASK_STK_SIZE - 1],LED1_TASK_PRIO);
	OSStart();
}

static void systick_init(void)
{
	RCC_ClocksTypeDef rcc_clocks;
	RCC_GetClocksFreq(&rcc_clocks);
	SysTick_Config(rcc_clocks.HCLK_Frequency/OS_TICKS_PER_SEC);

	LED_Init();
	delay_ms(100);
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�

	SPI1_Init();	 //��ʼ��SPI1  PA5 PA6 PA7 IOΪSPIģʽ
	SPI1_SetSpeed(SPI_BaudRatePrescaler_2);	 //����SPI1�ٶ�Ϊ���

	W5500_GPIO_Init();//��ʼ��W5500  RST INT SCS��ӦGPIO״̬ ������INT�ж�ģʽ
}

static void getCmd_task(void *p_arg)
{
	systick_init();	
	strcpy(iinfo.num,"963258");
	strcpy(iinfo.serPort,"9601");
	
	initSocket(sock_tcpSer);
	initServer(sock_ser);
	sendBaseDataAboutIphone();
 	while(1)
	{		
		if(recv(sock_tcpSer))
			{
				//get data
				getTcpSerCmd(socketInfo[sock_tcpSer].Rx_Buffer);
			}
		isCallIphone(scankey());
		WXCLED( ON );		
		OSTimeDlyHMSM(0,0,0, 300);
	}	
}

static void recv_task(void *p_arg)
{	
	p_arg = p_arg;
	while(1){
		WXCLED( OFF);  
		OSTimeDlyHMSM(0,0,0,400);			
		if(iinfo.status == IPHONECOMMON)
		{
			//get data about 
		}
		if(isCallUp ==1 )
		{
			// show arrnum to pingmu 
			//isCall up == 1 ��ʾ�Ǳ��� ����ʾ�Է��绰����
		}
			
	}
}






