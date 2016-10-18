#include "stdafx.h"

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <string>
#include <iostream>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Wsock32.lib")

using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
	WSAData wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		printf("error");
		return false;
	}
	SOCKET sock=socket(PF_INET,SOCK_DGRAM,0);
	sockaddr_in bindaddr;
	bindaddr.sin_family=AF_INET;
	bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bindaddr.sin_port=htons(3000);
	bind(sock,(sockaddr *)&bindaddr,sizeof(sockaddr_in));

	sockaddr_in signl_serv;
	signl_serv.sin_family=AF_INET;
	signl_serv.sin_addr.s_addr =inet_addr("93.170.168.253");
	//signl_serv.sin_addr.s_addr =inet_addr("192.168.56.101");

	signl_serv.sin_port=htons(30000);

	char ip_server[100];
	std::cout<<"recieve ip:port"<<endl;
	int gg = sendto(sock, "get", strlen("get"), 0,(sockaddr *)&signl_serv, sizeof(signl_serv));	
	sockaddr_in serv;
	socklen_t servlen=sizeof(sockaddr_in);

	int len=recvfrom(sock,ip_server,100,0,(SOCKADDR *)&serv,&servlen);
	ip_server[len] ='\0';
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
			i++;
		}
		sockaddr_in send_serv;
		send_serv.sin_family=AF_INET;
		send_serv.sin_addr.s_addr =inet_addr(ip.c_str());
		send_serv.sin_port=htons(atoi( sport.c_str()));
		char mess[] = "HELLO WORLD";
		int send = sendto(sock, mess, strlen(mess), 0,(sockaddr *)&send_serv, sizeof(send_serv));
		cout<<" message:"<<mess<<" sended to "<<ip_server;
		getchar();
	}
	WSACleanup();
	return 0;
}

