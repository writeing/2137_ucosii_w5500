
/**********************************************************************************
 * 系统时钟SysTick	 即滴答定时器
 * 描述：通过STM32的‘‘心脏’’
 * 库版本  ：ST_v3.5
**********************************************************************************/


#include "delay.h"


//static u8  fac_us=0;//us延时倍乘数
//static u16 fac_ms=0;//ms延时倍乘数



void delay_init()	 
{
//    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
//	fac_us=SystemCoreClock/8000000;	//为系统时钟的1/8  

//	fac_ms=(u16)fac_us*1000;//代表每个ms需要的systick时钟数 

}


//延时nus
//nus为要延时的us数.		    								   
void delay_us(u32 nus)
{		
   OSTimeSet(0);
	while(OSTimeGet()<1)
	{
	} 
}


//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864 
void delay_ms(u16 nms)
{	 		  	  
  OSTimeSet(0);
	while(OSTimeGet()<nms)
	{
	} 	    
} 

