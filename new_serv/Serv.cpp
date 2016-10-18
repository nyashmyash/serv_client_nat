#include "stdafx.h"
#include "Serv.h"
#include <stdio.h>
#include <ctime>
#include <string>
#include <iostream>


using namespace std;
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Wsock32.lib")

#define MAX_BUFFER_SIZE 1024

sockaddr_in stunserver={AF_INET,};


void bzero(void* mem, int size)
{
	memset(mem,0,size);
}

static void stun_send(stun_state *st) {

	sockaddr_in *dst;
	dst=&stunserver;
	char bufnew[] = 
		//Header:
	{		
		0x00,0x01,      
		0x00,0x08,     
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    

		0x00,0x03,     
		0x00,0x04,    
		0x00,0x00,0x00,0x00 
	};

	for(int i=0;i<16;i++)
		bufnew[i+4]=rand()%sizeof(char);
	sendto(st->sock, (char*)bufnew, 8 + sizeof(stun_header), 0,(sockaddr *)dst, sizeof(*dst));

}

std::string data_thread(stun_state *st) {
	int len;
	unsigned char *buf;
	socklen_t sinlen=sizeof(sockaddr_in);
	sockaddr_in sin;	

	buf=(unsigned char*)malloc(MAX_BUFFER_SIZE);
	bzero(buf,sizeof(buf));
	st->result=0;
	st->pcnt=0;
	char outip[100];
	{
		len=recvfrom(st->sock,(char*)buf,MAX_BUFFER_SIZE,0,(SOCKADDR *)&sin,&sinlen);
		if (len > 0) {
			int pos = 20;
			while(1){
				int attrsize=buf[pos+2]*256+buf[pos+3];
				if (buf[pos]!=0||buf[pos+1]!=1)	{
					pos+=attrsize;
					continue;
				}
				int port=buf[pos+6]*256+buf[pos+7];
				byte res[] = { buf[pos+8],buf[pos+9],buf[pos+10],buf[pos+11] };
				sprintf(outip,"%i.%i.%i.%i:%i", buf[pos+8],buf[pos+9],buf[pos+10],buf[pos+11] , port);
				break;
			}
		}
	}
	free(buf);
	return outip;
}


void getstun(stun_state &st, char* ip)  {
	struct hostent *hp;

	hp=gethostbyname("stun.l.google.com");
	memcpy(&stunserver.sin_addr, hp->h_addr, sizeof(stunserver.sin_addr));
	stunserver.sin_port = htons(19302);

	std::string retstr;
	while(1) {
		stun_send(&st);
		Sleep(5000);
		retstr = data_thread(&st);
		break;
	}

	strcpy(ip, retstr.c_str());
}

bool run()
{
	WSAData wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		printf("error");
		return false;
	}
	stun_state st;
	char ip_port[100]="";
	st.sock=socket(PF_INET,SOCK_DGRAM,0);

	st.bindaddr.sin_family=AF_INET;
	st.bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	st.bindaddr.sin_port=htons(3000);
	bind(st.sock,(sockaddr *)&st.bindaddr,sizeof(sockaddr_in));
	getstun(st, ip_port) ;
	std::cout<<"nat ip:port of server  "<<ip_port;
	sockaddr_in signl_serv;
	bzero(&signl_serv, sizeof(signl_serv));
	signl_serv.sin_family=AF_INET;
	signl_serv.sin_addr.s_addr =inet_addr("93.170.168.253");
	//signl_serv.sin_addr.s_addr =inet_addr("192.168.56.101");
	signl_serv.sin_port=htons(30000);
	//strcat(ip_port,":Server" );
	char ip_server[100];
	int sended = sendto(st.sock, ip_port, strlen(ip_port), 0,(sockaddr *)&signl_serv, sizeof(signl_serv));	

	while (1)
	{
		char mess[100];
		sockaddr_in serv;
		socklen_t servlen=sizeof(sockaddr_in);
		int len=recvfrom(st.sock,mess,100,0,(SOCKADDR *)&serv,&servlen);
		std::cout<<"message:"<<mess;
		getchar();
	}
	/*sockaddr_in serv;
	socklen_t servlen=sizeof(sockaddr_in);

	int len=recvfrom(st.sock,ip_server,100,0,(SOCKADDR *)&serv,&servlen);
	if (len > 0) {
		int i =0;
		string ip="";
		string sport ="";
		while(ip_server[i]!=':'){
			ip+=ip_server[i]; i++;
		}
		i++;
		while (i<len){
			sport+=ip_server[i];
		}
		sockaddr_in send_serv;
		send_serv.sin_family=AF_INET;
		send_serv.sin_addr.s_addr =inet_addr(ip.c_str());
		send_serv.sin_port=htons(atoi( sport.c_str()));
		char mess[] = "HELLO WORLD";
		sendto(st.sock, mess, strlen(mess), 0,(sockaddr *)&send_serv, sizeof(&send_serv));	
	}*/
	closesocket(st.sock);
	WSACleanup();
	return true;

}

