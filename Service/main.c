#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "utils.h"

#define GameStart '9'
#define NewPlayer '1' 
#define KeyPress '2'
#define KeyRelease '3'
#define Ready '4'

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

char* msgList[1024]; 
int msgs=0;
int players=0; 
int MSGSIZE=32;
int isReady[4];
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
			printf("send:%s\n",buffer);
		}	
	}
}
void * getMsg(void *ptr){
	SOCKET* client=(SOCKET*)ptr;
	char msg[MSGSIZE];
	while(1){
		memset(msg, 0, sizeof(msg));
		recv(*client,msg,MSGSIZE,0);
		if(0){//msg[0]==GameStart
			sprintf(msg,"%d$%d",KeyPress,2);
			sendTo(client,msg);
			printf("Press!\n");
			sleep(1);
			sprintf(msg,"%d$%d",KeyRelease,2);
			sendTo(client,msg);
			printf("Release!\n");
			continue;
		}
		else if(msg[0]==Ready){
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
				if(res){
					printf("gamstart!\n");
					
					sprintf(msg,"%c",GameStart);
					sendMsg(msg);
				}	
			}
			else{
				printf("%s$\n",msg);
			}
				
		}
		//printf("%dst msg is %s\n",msgs,buffer);
	}
	printf("end\n");
}

/*void * sendMsg(void *ptr){
	printf("start send msg\n");
	while(1){
		printf("msgs:%d players:%d\n",msgs,players);
		if(msgs>0){
			printf("msg:%c\n",msgList[0]);
		}
		if(players>0){
			if(msgs>0){
				for(int i=0;i<msgs;i++){
					for(int j=0;j<players;j++){
						if(&playerList[j]==NULL)break;
						send(playerList[j],msgList[i],MSGSIZE,0);
						//printf("send:%s\n",msgList[i]);
					}
				}
			}
			msgs=0;	
		}
		
		sleep(2);//send per 1s
	}
}*/



void handleDate(char* buffer){
	int pname=0;
}


int main(int argc, char *argv[]){
	
	
    
    startSock();
    
    initArgs(argc, argv);
    
    initSock(globalArgs.port);
    
    pthread_t ids;
    pthread_t id;
    int res;
    //res=pthread_create(&id,NULL,sendMsg,NULL);
//    if(!res){
//    	printf("thread send error\n");
//    	exit(1);
//	}
    char buffer[MSGSIZE];
    sprintf(buffer,"%d",0);
    //strncpy(infoList[players].id,buffer,1);
    while(1){
    	int nSize;
    	SOCKADDR playerAddr;
    	nSize = sizeof(SOCKADDR);
    	SOCKET pClient = accept(servSock, (SOCKADDR*)&playerAddr, &nSize);
    	
    	playerList[players]=pClient;
    	
    	
		sprintf(buffer,"%d$",players+1);
		strncpy(infoList[players].id,buffer,1);
		//printf("%s",infoList[players].id);
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
    	printf("recv!\n");
		for(int i=0;i<MSGSIZE;i++){
			if(buffer[i]!='\0')continue;
			strncpy(infoList[players].username,buffer,i);
			break;
		}
		
		
		if(players>0){
			for(int j=0;j<players;j++){
				if(&playerList[j]==NULL)break;//Ã»ÂÑÓÃ? 
					sprintf(buffer,"%d$%s$%s$",NewPlayer,infoList[players].username,infoList[players].id);
					send(playerList[j],infoList[players].username,MSGSIZE,0);
			}
		}
		
		printf("Player:%s,Connected\n",infoList[players].username);
		players++;
		
		
		
    	pthread_create(&ids,NULL,getMsg,&pClient);
    	
	}
    
    
    //printf("%s",infoList[players].id);	
    closeSock();
    
    return 0;
}
