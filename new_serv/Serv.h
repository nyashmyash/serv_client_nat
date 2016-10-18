#pragma once
#include <winsock2.h>
#include <Ws2tcpip.h>
#define BUFSIZE 100		
#define PORT 3000	


#pragma pack (push)
#pragma pack (16)
struct stun_header {
	unsigned short msgtype;
	unsigned short msglen;
	unsigned int id[4];
	unsigned char ies[0];
} ;
#pragma pack (pop)

#pragma pack (push)
#pragma pack (16)
struct stun_state {
	char *username;
	char *password;
	sockaddr_in caddr,maddr;
	int result,pcnt;
	timeval laststun;
	sockaddr_in bindaddr; //bindaddr ast_rtp->us
	int sock; //ast_rtp->s
};
#pragma pack (pop)
bool run();

