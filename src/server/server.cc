#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include "../tetrisSingle.h"
#include "../tetrisMulti.h"
#define MAX_CLNT    2


void * Handle(void * arg);
void SendData(int sock_cli, char * msg, int len);
void Error(const char * msg);

int clnt_cnt=0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t m, mu;

int main()
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	pthread_t t_id;
  
	pthread_mutex_init(&m, NULL);
	pthread_mutex_init(&mu, NULL);
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET; 
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(WELL_KNOWN_PORT);
	
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		Error("BIND ERROR!!\n wait a minute.\n");
	if(listen(serv_sock, 2)==-1)
		Error("LISTEN ERROR!!");
	
	while(1)
	{
		clnt_adr_sz=sizeof(clnt_adr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, (socklen_t *)&clnt_adr_sz);
		
		pthread_mutex_lock(&m);
		clnt_socks[clnt_cnt++]=clnt_sock;
		pthread_mutex_unlock(&m);
	
		pthread_create(&t_id, NULL, Handle, (void*)&clnt_sock);
		pthread_detach(t_id);
        std::cout << "Connected client IP: " << inet_ntoa(clnt_adr.sin_addr) << std::endl;
	}
	close(serv_sock);
	return 0;
}
	
void * Handle(void * arg)
{
    int clnt_sock = *((int*)arg);
    char data[BUF_SIZE];
    int len;

	while((len=read(clnt_sock, data, sizeof(data)))!=0) {
        pthread_mutex_lock(&mu);
        //error detect
        if(len != BUF_SIZE) continue;
        if(data[BUF_SIZE-1] != '9') continue; 

        SendData(clnt_sock, data, len);
        pthread_mutex_unlock(&mu);
    }

	pthread_mutex_lock(&m);
	for(int i=0; i<clnt_cnt; i++)   // remove disconnected client
	{
		if(clnt_sock==clnt_socks[i])
		{
			while(i++<clnt_cnt-1)
				clnt_socks[i]=clnt_socks[i+1];
			break;
		}
	}
	clnt_cnt--;
	pthread_mutex_unlock(&m);

	close(clnt_sock);
	return NULL;
}

void SendData(int clnt_sock, char * data, int len)
{
	pthread_mutex_lock(&m);
	for(size_t i=0; i<clnt_cnt; i++) {
	    if(clnt_sock == clnt_socks[i]) continue;
        write(clnt_socks[i], data, len);
    }
	pthread_mutex_unlock(&m);
}

void Error(const char * msg)
{
	std::cout << msg << std::endl;
	exit(1);
}
