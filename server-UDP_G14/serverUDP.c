/*
 * serverUDP.c
 *
 *  Created on: 17 dic 2025
 *      Author: Andrea Martilotti
 *      Author: Michele Ritella
 */

#include <stdio.h>
#include <string.h>
#include <winsock.h>


int main(){

	WSADATA wsaData;
	WORD wVersionRequested;
	wVersionRequested= MAKEWORD(2,2);
	int iResult = WSAStartup(wVersionRequested, &wsaData);
	if (iResult != 0) {
		printf("Error at WSAStartup()\n");
		printf("A usable WinSock DLL cannot be found");
		return -1;
	}

	int ServerSocket;
	struct sockaddr_in echoServAddr;
	struct sockaddr_in echoClntAddr;

	if(ServerSocket=socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP))<0){
		perror("\nSocket creation Error")
	}



	return 0;
}
