
#include "Server.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>

udp_server::udp_server(const std::string& addr, int port)
    : f_port(port)
    , f_addr(addr)
{
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(sockaddr));  
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    f_socket = socket(PF_INET, SOCK_DGRAM, 0);
    bind(f_socket, (struct sockaddr*) &servaddr, sizeof(servaddr));

}

udp_server::~udp_server()
{
    close(f_socket);
}


int udp_server::get_socket() const
{
    return f_socket;
}

int udp_server::get_port() const
{
    return f_port;
}


std::string udp_server::get_addr() const
{
    return f_addr;
}


int main(int argc, char ** argv)
{
    udp_server serv = udp_server("",30000);
    char ip[100];   
    while(1)
	{
		char mes[100];
		int size = 100;	
		std::string str;		
		struct sockaddr_in inrec;
		socklen_t servlen = sizeof(sockaddr_in);
    		int cnt = recvfrom(serv.get_socket(), mes, size, 0, (sockaddr*)&inrec, &servlen);
		str = mes;
		if ( str == "get")
		{	
			std::cout<<ip;
			int cnt = sendto(serv.get_socket(), ip, strlen(ip), 0, (sockaddr*)&inrec, sizeof(inrec));	
		} 
		else
		{
			strcpy(ip, mes);
			//std::cout<<ip;
						
		}
		
	}

}

