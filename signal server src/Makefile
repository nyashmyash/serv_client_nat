# Makefile for the socket programming example
#

server_objects = Server.o


all : simple_server

simple_server: $(server_objects)
	g++ -o simple_server $(server_objects)

udp_client_server_runtime_error: Server.h
udp_server: Server.cpp



clean:
	rm -f *.o simple_server
