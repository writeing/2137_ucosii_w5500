
#include "Main.h"
void initFTP()
{	
	printf("connnet FTP server %s\r\n",gettargetIP(0));
	recv(sock_control);
	//Process_Socket_Data(0);	
	//ftclient_login();
}
struct command recvCmd[20];
int indexRecvCmd = 0;
void getUsartCmd()
{
	if(rev_flag)
	{
		//Write_SOCK_Data_Buffer(sock_control, (unsigned char*)"nimabi", 7);	
			if(!strcmp((char *)USART_RX_BUF,"LIST"))
				{
					strcpy(recvCmd[indexRecvCmd++].code,(char *)"PASV");
					if(indexRecvCmd == 20)
						indexRecvCmd = 0;
					strcpy(recvCmd[indexRecvCmd++].code,(char *)USART_RX_BUF);					
				}
			else
				if(strstr((char *)USART_RX_BUF,"RETR"))
				{
					strcpy(recvCmd[indexRecvCmd++].code,(char *)"SIZE");
					if(indexRecvCmd == 20)
						indexRecvCmd = 0;
					strcpy(recvCmd[indexRecvCmd++].code,(char *)"PASV");
					if(indexRecvCmd == 20)
						indexRecvCmd = 0;
					strcpy(recvCmd[indexRecvCmd++].code,(char *)USART_RX_BUF);					
				}
				else
				// if (strstr((char *)USART_RX_BUF," ")) 
				{
					strcpy(recvCmd[indexRecvCmd++].code,(char *)USART_RX_BUF);
				}
		if(indexRecvCmd == 20)
					indexRecvCmd = 0;
		rev_flag = 0;
		memset(USART_RX_BUF,'\0',strlen((char *)USART_RX_BUF));		
	}
}

void setFtpCmd(int rececode)
{
	static int fileSize = 0;
	switch(rececode)
	{
		case 220:			
		case 331:
		case 230:
		case 257:
			printf("%s\r\n",socketInfo[0].Rx_Buffer);		
			break;
		case 227:
			// PASV
			getDataPort(socketInfo[0].Rx_Buffer);
			connectFtpData(sock_data);
			break;
		case 250:
			//get data;
			// CWD
			ftclient_get(fileSize,NULL);
			fileSize = 0;			
			break;
		case 213:
			//get file size
			//SIZE
			fileSize = ftpclient_getfilesize(socketInfo[0].Rx_Buffer);
			printf("size = %d\r\n",fileSize);
			break;
		case 150:		
			//RETR  LIST
			//start get file data
			ftclient_get(fileSize,NULL);			
			ftpCloseDataCon();
			fileSize = 0;
			break;
		case 226:
			//tar finash close sock			
			printf("Transfer complete\r\n");
			break;
		case 125:
			//get list data
			//printf("%s\r\n",socketInfo[0].Rx_Buffer);
			ftclient_get(fileSize,NULL);
			ftpCloseDataCon();
			fileSize = 0;
			break;
//		case 150:
//			//get file data
//			break;			
		default :
		printf("%d : %s\r\n",rececode,socketInfo[0].Rx_Buffer);
	}
}


