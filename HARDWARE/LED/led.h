
#ifndef	__LED_H
#define __LED_H
#include "stm32f10x.h"

void LED_Init(void);//初始化

void led_pa1(u8 sta);
#define ON  0
#define OFF 1

//带参宏，可以像内联函数一样使用
#define WXCLED(a)	if (a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_1);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_1)

#endif
