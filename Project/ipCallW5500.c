#include "ipCallW5500.h"

IphoneInfo iinfo;



int sock_tcpSer = 0;
int sock_tcpIphone = 1;
int sock_ser = 2;
int sock_udp = 3;

int isCallUp = 0;  // 0 null  1: has call up
int getCmdType(u8 *data)
{
	char cmdCode[][8] = {"COM0","COM1","COM2","COM3","COM4","COM5","COM6","COM7","COM8","COM9","COM10"};
	int i = 0;
	for(i = 0 ; i < 11; i ++)
		{
			if(strstr((char *)data,(char *)cmdCode[i]))
			{
				return 10+i;
			}				
		}
	return 0;
}
// 12:96.63    
void getSerPort(u8 *data)
{
	char buff[20];
	char *temp;
	memset(buff,'\0',20);
	strcpy((char *)buff,(char *)data);
	temp = strtok(buff,(char *)":");
	temp = strtok(NULL,(char *)":");
	//serPort = atoi(temp);
	strcpy((char *)socketInfo[sock_tcpIphone].info.Port,temp); //把端口保存在tcpiphone链表里面
	//strcpy((char *)iinfo.serPort,temp);
}
// 13:0
void getCallStatus(u8 *data)
{

	char buff[20];
	char *temp;
	int iphoneStatus = 0;
	memset(buff,'\0',20);
	strcpy((char *)buff,(char *)data);
	temp = strtok(buff,(char *)":");
	temp = strtok(NULL,(char *)":");
	iphoneStatus = atoi(temp);
	if(iphoneStatus == 2)
		iinfo.status = IPHONENORMAL;	
	if(iphoneStatus == 0)
		{
			iinfo.status = IPHONENORMAL;
			//close sock_tcpIphone connect
			tcpCloseDataCon(sock_tcpIphone);
		}
	if(iphoneStatus == 1)
		{
			iinfo.status = IPHONECOMMON;
		}
	
	
}
//14:192.168.1.1:8888
void gettcpinfo(u8 *data)
{

	char buff[100];
	char *temp;

	memset(buff,'\0',100);
	strcpy((char *)buff,(char *)data);
	temp = strtok(buff,(char *)":");	// 14

	temp = strtok(NULL,(char *)":");
	strcpy((char *)iinfo.ip,temp);	//ip
	
	temp = strtok(NULL,(char *)":");
	strcpy((char *)iinfo.num,temp);	//num
}
//17:192.168.1.1:XX.XX:99.32
void getRemoteInfo(u8 *data)
{

		char buff[100];
		char *temp;

		memset(buff,'\0',100);
		strcpy((char *)buff,(char *)data);
		temp = strtok(buff,(char *)":");	// 17
	
		temp = strtok(NULL,(char *)":");
		strcpy((char *)socketInfo[sock_tcpIphone].info.DIP,temp);	//Dip
		
		temp = strtok(NULL,(char *)":");
		strcpy((char *)socketInfo[sock_tcpIphone].info.Port,temp);	//port


		temp = strtok(NULL,(char *)":");
		strcpy((char *)socketInfo[sock_tcpIphone].info.DPort,temp);	//Dport


}
void sendBaseDataAboutIphone()
{
	//extern void Write_SOCK_Data_Buffer(SOCKET s, unsigned char *dat_ptr, unsigned short size);
	char buf[20];
	
	
	memset(buf,'\0',20);
	sprintf(buf,"NUM:%s",iinfo.num);
	send(sock_tcpSer,(u8 *)buf);
	
	memset(buf,'\0',20);
	sprintf(buf,"SERPORT:%s",iinfo.serPort);
	send(sock_tcpSer,(u8 *)buf);
	
}


void initSocket(SOCKET sock)
{
	initW5500_tcpSer(sock);			//初始化 info信息 
	init_w5500();	  //本机地址 物理地址 初始化	
	setTCPSocketinfo(sock,TCP_CLIENT);		//初始化socket连接信息
	W5500_connect(sock);				//连接函数
}

void getRemoteNum(u8* data)
{
	char buff[20];
	char *temp;	
	memset(buff,'\0',20);
	strcpy((char *)buff,(char *)data);
	temp = strtok(buff,(char *)":");
	temp = strtok(NULL,(char *)":");
	strcpy((char *)arrNum,temp);
}
int getTcpSerCmd(u8 *data)
{
	int recode = 0;
	recode = getCmdType(data);
	printf("recode = %d\r\n",recode);
	switch(recode)
	{
	  	case 10:
			//通话请求//获取对方的num
			getRemoteNum(data);
			isCallUp = 1;
			break;
		case 11:
			//not send num and serPort
			sendBaseDataAboutIphone();
			break;
		case 12:			
			// recv hang up
			isCallUp = 0;
			iinfo.status = IPHONENORMAL;
			//close tcpiphone connect 
			tcpCloseDataCon(sock_tcpIphone);
			memset(arrNum,'\0',20);
			break;
		case 13:
			//get connect status
			getCallStatus(data); //2:call false   0:remote ok  1: remote false
			break;
		//i don't know top two cmd what to do   but  i create
		case 14:
			//updata tcp info ip,num
			gettcpinfo(data);
			//重新连接
			init_w5500();	  //本机地址 物理地址 初始化			
			setTCPSocketinfo(sock_tcpSer,TCP_CLIENT);		//初始化socket连接信息
			W5500_connect(sock_tcpSer);				//连接函		
			//server reopen			
			setTCPSocketinfo(sock_ser,TCP_SERVER);		//初始化socket连接信息
			W5500_connect(sock_ser);				//连接函		
			break;
		case 17:
			//get remote port and ip
			getRemoteInfo(data);
			//connect remote server
			setTCPSocketinfo(sock_tcpIphone,TCP_CLIENT);		//初始化socket连接信息
			W5500_connect(sock_tcpIphone);				//连接函数			
			send(sock_tcpSer,"CONNECT 1");  //连接成功
			break;
		case 15:
			// call success   next send or resv data
			//iinfo.status = IPHONECOMMON;			//正在通话
			break;			
		case 16:
			// call false
			//iinfo.status = IPHONENORMAL;			//normal
			break;			
		default:break;
	}
	return 0;
}


char arrNum[20] = {0};		//sava num to call
int scankey()
{
	return NORMAL;
}

void closeSerSocket()
{
	
}

void initServer(SOCKET sock)
{
	strcpy((char *)socketInfo[sock].info.Port,(char *)iinfo.serPort);
	strcpy((char *)socketInfo[sock].info.DPort,(char *)"");
	strcpy((char *)socketInfo[sock].info.DIP,(char *)"");	
	setTCPSocketinfo(sock,TCP_SERVER);		//初始化socket连接信息
	W5500_connect(sock);				//连接函数
}


void isCallIphone(int status)
{
	switch(status)
	{
		case NORMAL:
			break;
		case CALLSTATUS:
			if(isCallUp)
				{
					//被呼叫，选择接听
					//sock_ser   send: 15:ok
					//sock_tcpSer receive

					send(sock_tcpSer,(u8 *)"receive");
					iinfo.status = IPHONECOMMON;		//正在通话中
				}
			else
				{
					//呼叫别人
					//sock_tcpSer  CALL:num
					sprintf((char *)socketInfo[0].Tx_Buffer,"CALL:%s",arrNum);
					send(sock_tcpSer,socketInfo[0].Tx_Buffer);					
					iinfo.status = IPHONECALLING;		//正在呼叫
				}
			break;
		case REJECT:
			//拒绝连接
			closeSerSocket();
			//send sock_tcpSer  NO
			//send sock_ser  16:no
			isCallUp = 0;
			send(sock_tcpSer,(u8 *)"no");
			iinfo.status = IPHONENORMAL;
			break;
	}
}


