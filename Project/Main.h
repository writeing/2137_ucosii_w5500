#ifndef __MAIN_H_
#define __MAIN_H_
#include  "delay.h"
#include  "led.h"
#include  "usart.h"
#include  "spi.h"
#include  "W5500.h"
#include  "string.h"
#include 	"ftclient.h"
void initFTP(void);
extern struct command recvCmd[20];
extern  int indexRecvCmd;
void getUsartCmd(void );
void setFtpCmd(int rececode);




#endif



