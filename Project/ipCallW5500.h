#ifndef __IPCALLW5500_H_
#define __IPCALLW5500_H_
#include "W5500.h"

#define NORMAL      0x0
#define CALLSTATUS  0x1 
#define REJECT      0x2



#define IPHONENORMAL    0x0
#define IPHONECALLING   0x1
#define IPHONECOMMON    0x3

typedef struct iphoneinfo
{
	char ip[20];
	char num[20];
	char serPort[10];
	char RemoteserPort[10];	
	char status;	
}IphoneInfo;
extern IphoneInfo iinfo;
extern char arrNum[20];
extern int isCallUp;


int getTcpSerCmd(u8 *data);
void sendBaseDataAboutIphone(void);

int scankey(void );

void isCallIphone(int status);
 

void initServer(unsigned char sock);


void initSocket(unsigned char sock);

#endif

