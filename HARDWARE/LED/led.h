
#ifndef	__LED_H
#define __LED_H
#include "stm32f10x.h"

void LED_Init(void);//��ʼ��

void led_pa1(u8 sta);
#define ON  0
#define OFF 1

//���κ꣬��������������һ��ʹ��
#define WXCLED(a)	if (a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_1);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_1)

#endif
