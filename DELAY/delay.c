
/**********************************************************************************
 * ϵͳʱ��SysTick	 ���δ�ʱ��
 * ������ͨ��STM32�ġ������࡯��
 * ��汾  ��ST_v3.5
**********************************************************************************/


#include "delay.h"


//static u8  fac_us=0;//us��ʱ������
//static u16 fac_ms=0;//ms��ʱ������



void delay_init()	 
{
//    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
//	fac_us=SystemCoreClock/8000000;	//Ϊϵͳʱ�ӵ�1/8  

//	fac_ms=(u16)fac_us*1000;//����ÿ��ms��Ҫ��systickʱ���� 

}


//��ʱnus
//nusΪҪ��ʱ��us��.		    								   
void delay_us(u32 nus)
{		
   OSTimeSet(0);
	while(OSTimeGet()<1)
	{
	} 
}


//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864 
void delay_ms(u16 nms)
{	 		  	  
  OSTimeSet(0);
	while(OSTimeGet()<nms)
	{
	} 	    
} 

