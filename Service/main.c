#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "utils.h"

#define DisConnect '0'
#define NewPlayer '1' 
#define KeyPress '2'
#define KeyRelease '3'
#define Ready '4'
#define Prop '8'
#define GameStart '9'

struct globalArgs_t {
    int port;
} globalArgs;

static const char *optString = "p:h";

typedef struct PlayerInfo{
	char username[12];
	char id[1];
}playerInfo;

SOCKET servSock;
//char buffer[MAXBYTE] = {0};
SOCKET playerList[4];
playerInfo infoList[4];

//char* msgList[1024]; 
int msgs=0;
int players=0; 
int MSGSIZE=32;
int isReady[4];
int isAlive[4];
int isConnect[4];
int gaming=0;
/*
 * Ip Utils
 */
 
BOOL isPort(const int port)
{
    return (port >= 0 && port <= 65535);
}


char *getIp()
{
    PHOSTENT hostinfo;
    char name[255];
    char* ip;
    if(gethostname(name, sizeof(name)) == 0)
    {
        if((hostinfo = gethostbyname(name)) != NULL)
        {
            ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
            return ip;
        }
    }
    return NULL;
} 

void sendTo(SOCKET *sock, const char *message)
{
    send(*sock, message, strlen(message) + sizeof(char), 0);
    Sleep(100);
}


void startSock()
{
    // Initial DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
}

void initSock(int port)
{
    // Open socket
    servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    // Bind socket
    struct sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = PF_INET;  // Use ipv4 address 
    sockAddr.sin_addr.s_addr = inet_addr("0.0.0.0");  // Bind to LAN
    sockAddr.sin_port = htons(port);  // Bind port
    bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
    
    // Listen
    listen(servSock, 20);
    
    char *ip = getIp();
    if (NULL != ip)
    {
        printf("Listening on %s:%d\n", ip, port);
    }
    else
    {
        printf("Listening...\n");
    }
       
}

void closeSock()
{
    // Close socket
    for(int i=0;i<players;i++){
    	closesocket(playerList[players]);
	}
    closesocket(servSock);
    
    // Close DLL
    WSACleanup();
}



void display_usage(char *exe)
{
    printf("Usage: %s [OPTIONS] \n", exe);
    printf("  -p port           Server port\n");
}

void initArgs(int argc, char *argv[])
{
    int opt = 0;
    globalArgs.port = 2111;
   
    opt = getopt(argc, argv, optString);
    while (opt != -1)
    {
        switch (opt)
        {
            case 'p':
                globalArgs.port = atoi(optarg);
                break;
            case 'h':
                display_usage(argv[0]);
                exit(0);
                break;
            default:
                // Illegal!
                break;
        }
        
        opt = getopt(argc, argv, optString);
    }
}

void sendMsg(char* buffer){
	if(players>0){
		for(int j=0;j<players;j++){
			if(&playerList[j]==NULL)break;
			send(playerList[j],buffer,MSGSIZE,0);
		}
		printf("send:%s\n",buffer);	
	}
}
void resetStatus(){
	for(int i=0;i<4;i++){
		isReady[i]=0;
    	isAlive[i]=1;
    	isConnect[i]=1;
	}
}
void * getMsg1(void *ptr){
	SOCKET* client=&playerList[0];
	char msg[MSGSIZE];
	
	while(1){
		memset(msg, 0, sizeof(msg));
		recv(*client,msg,MSGSIZE,0);
		printf("Recv from %s(%sst):%s\n",infoList[0].username,infoList[0].id,msg);
		if(msg[0]==Ready){
			int id=(int)msg[2]-49;
			if(id>=0&&id<=3){
				if(msg[4]=='r'){
					isReady[id]=1;
					printf("%dst is ready!\n",id+1);
				}
				else {
					isReady[id]=0;
					printf("%dst is unready!\n",id+1);
				}
				int res=1;
				for(int i=0;i<players;i++){
					res*=isReady[i];
				}
				if(res&&players>1){
					printf("gamstart!\n");
					gaming=1;
					sprintf(msg,"%c",GameStart);
					sendMsg(msg);
				}	
			}
			else{
				printf("%s$\n",msg);
			}
				
		}
		else if(msg[0]=='d'){
			int id=(int)msg[2]-49;
			isAlive[id]=0;
			int res=0;
			for(int i=0;i<players;i++){
				res+=isAlive[i];
			}
			if(res==1){
				gaming=0;
				resetStatus();
			}
		}
		else if(msg[0]==DisConnect){
			int id=(int)msg[2]-49;
			isConnect[id]=0;
			printf("%dst is disconnected!\n",id+1);
			break;
		}
		else{
			sendMsg(msg);
		}
	}
}
void * getMsg2(void *ptr){
	SOCKET* client=&playerList[1];
	char msg[MSGSIZE];
	//printf("getMsg2\n");
	while(1){
		memset(msg, 0, sizeof(msg));
		recv(*client,msg,MSGSIZE,0);
		printf("Recv from %s(%sst):%s\n",infoList[0].username,infoList[0].id,msg);
		if(msg[0]==Ready){
			int id=(int)msg[2]-49;
			if(id>=0&&id<=3){
				if(msg[4]=='r'){
					isReady[id]=1;
					printf("%dst is ready!\n",id+1);
				}
				else {
					isReady[id]=0;
					printf("%dst is unready!\n",id+1);
				}
				int res=1;
				for(int i=0;i<players;i++){
					res*=isReady[i];
				}
				if(res&&players>1){
					printf("gamstart!\n");
					gaming=1;
					sprintf(msg,"%c",GameStart);
					sendMsg(msg);
				}	
			}
			else{
				printf("%s$\n",msg);
			}
		}
		else if(msg[0]=='d'){
			int id=(int)msg[2]-49;
			isAlive[id]=0;
			int res=0;
			for(int i=0;i<players;i++){
				res+=isAlive[i];
			}
			if(res==1){
				gaming=0;
				resetStatus();
			}
		}
		else if(msg[0]==DisConnect){
			int id=(int)msg[2]-49;
			isConnect[id]=0;
			printf("%dst is disconnected!\n",id+1);	
			break;
		}
		else{
			sendMsg(msg);
		}
	}
}
void * getMsg3(void *ptr){
	SOCKET* client=&playerList[2];
	char msg[MSGSIZE];
	while(1){
		memset(msg, 0, sizeof(msg));
		recv(*client,msg,MSGSIZE,0);
		printf("Recv from %s(%sst):%s\n",infoList[0].username,infoList[0].id,msg);
		if(msg[0]==Ready){
			int id=(int)msg[2]-49;
			if(id>=0&&id<=3){
				if(msg[4]=='r'){
					isReady[id]=1;
					printf("%dst is ready!\n",id+1);
				}
				else {
					isReady[id]=0;
					printf("%dst is unready!\n",id+1);
				}
				int res=1;
				for(int i=0;i<players;i++){
					res*=isReady[i];
				}
				if(res&&players>1){
					printf("gamstart!\n");
					gaming=1;
					sprintf(msg,"%c",GameStart);
					sendMsg(msg);
				}	
			}
			else{
				printf("%s$\n",msg);
			}	
		}
		else if(msg[0]=='d'){
			int id=(int)msg[2]-49;
			isAlive[id]=0;
			int res=0;
			for(int i=0;i<players;i++){
				res+=isAlive[i];
			}
			if(res==1){
				gaming=0;
				resetStatus();
			}
		}
		else if(msg[0]==DisConnect){
			int id=(int)msg[2]-49;
			isConnect[id]=0;
			printf("%dst is disconnected!\n",id+1);	
			break;
		}
		else{
			sendMsg(msg);

		}
	}
}
void * getMsg4(void *ptr){
	SOCKET* client=&playerList[3];
	char msg[MSGSIZE];
	while(1){
		memset(msg, 0, sizeof(msg));
		recv(*client,msg,MSGSIZE,0);
		printf("Recv from %s(%sst):%s\n",infoList[0].username,infoList[0].id,msg);
		if(msg[0]==Ready){
			int id=(int)msg[2]-49;
			if(id>=0&&id<=3){
				if(msg[4]=='r'){
					isReady[id]=1;
					printf("%dst is ready!\n",id+1);
				}
				else {
					isReady[id]=0;
					printf("%dst is unready!\n",id+1);
				}
				int res=1;
				for(int i=0;i<players;i++){
					res*=isReady[i];
				}
				if(res&&players>1){
					printf("gamstart!\n");
					gaming=1;
					sprintf(msg,"%c",GameStart);
					sendMsg(msg);
				}	
			}
			else{
				printf("%s$\n",msg);
			}
				
		}
		else if(msg[0]=='d'){
			int id=(int)msg[2]-49;
			isAlive[id]=0;
			int res=0;
			for(int i=0;i<players;i++){
				res+=isAlive[i];
			}
			if(res==1){
				gaming=0;
				resetStatus();
			}
		}
		else if(msg[0]==DisConnect){
			int id=(int)msg[2]-49;
			isConnect[id]=0;
			printf("%dst is disconnected!\n",id+1);	
			break;
		}
		else{
			sendMsg(msg);
		}
	}
}
void* (*funcList[4])(void *);


void* MakeProp(void *ptr){
	while(1){
		if(!gaming)continue;
		int x=rand()%1280;
		int y=rand()%1280;
		int type=rand()%14;
		if(type>6)type=6; 
		char buffer[MSGSIZE];
		sprintf(buffer,"%c$%d$%d$%d",Prop,x,y,type);
		printf("Prop:%s\n",buffer);
		sendMsg(buffer);
		sleep(3);
	}
}




int main(int argc, char *argv[]){
	
	srand((unsigned)time(0));
    
    startSock();
    
    initArgs(argc, argv);
    
    initSock(globalArgs.port);
    
   
    pthread_t id;
    pthread_create(&id,NULL,MakeProp,NULL);


    char buffer[MSGSIZE];
    sprintf(buffer,"%d",0);
    funcList[0]=&getMsg1;
    funcList[1]=&getMsg2;
    funcList[2]=&getMsg3;
    funcList[3]=&getMsg4;
    
    for(int i=0;i<4;i++){
    	isAlive[i]=1;
    	isConnect[i]=1;
	}

    for(int i=0;i<4;i++){
    	pthread_t ids;
    	
    	int nSize;
    	SOCKADDR playerAddr;
    	
    
    	nSize = sizeof(SOCKADDR);
    	SOCKET pClient = accept(servSock, (SOCKADDR*)&playerAddr, &nSize);
    	
    	
    	playerList[players]=pClient;
    	
		sprintf(buffer,"%d$",players+1);
		strncpy(infoList[players].id,buffer,1);

		if(players>0){
			for(int i=0;i<players;i++){
				strcat(buffer,infoList[i].username);
				strcat(buffer,"$");
				strcat(buffer,infoList[i].id);
				strcat(buffer,"$");
			}
		}
		sendTo(&pClient,buffer);
		
    	recv(pClient,buffer,MSGSIZE,0);

		for(int i=0;i<MSGSIZE;i++){
			if(buffer[i]!='\0')continue;
			strncpy(infoList[players].username,buffer,i);
			break;
		}
		
		if(players>0){
			for(int j=0;j<players;j++){
				sprintf(buffer,"%c$%s$%s$",NewPlayer,infoList[players].username,infoList[players].id);
				send(playerList[j],buffer,MSGSIZE,0);
			}
		}
		
		printf("%s(%sst),Connected\n",infoList[players].username,infoList[players].id);
		players++;
		
		
    	pthread_create(&ids,NULL,funcList[i],&pClient);

	}
    
    
    closeSock();
    
    return 0;
}
