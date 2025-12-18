/*
 * clientUDP.c
 *
 *  Created on: 18 dic 2025
 *      Author: Andrea Martilotti
 *      Author: Michele Ritella
 */


#include <stdio.h>
#include <string.h>
#include <winsock.h>
#define MAX_LEN 50


int main(){

	WSADATA wsaData;
	WORD wVersionRequested;
	wVersionRequested= MAKEWORD(2,2);
	int iResult = WSAStartup(wVersionRequested, &wsaData);
	if (iResult != 0) {
		printf("Error at WSAStartup()\n");
		return -1;
	}
	int porta;
	char name[MAX_LEN];

	printf("\n\nInserisci il nome del server:\n");
	scanf("%s", name);
	printf("\n\nInserisci la porta del server:\n");
	scanf("%d", &porta);

	int ClientSocket;
	if((ClientSocket=socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP))<0){
		printf("\n\nSocket Creation Failed\n");
		return -1;
	}
	struct hostent *ServerHost;
	ServerHost=gethostbyname(name);// prendere nome da input
	if(ServerHost != NULL){
		char messaggio[50];
		strcpy(messaggio, "Hello");
		struct in_addr* inAddr = (struct in_addr *)ServerHost->h_addr_list[0];
		printf("\n\nhost: %s", inet_ntoa(*inAddr));

		struct sockaddr_in ServerAddr;
		ServerAddr.sin_family=AF_INET;
		ServerAddr.sin_addr=*inAddr;
		ServerAddr.sin_port=htons(porta);
		printf("\n\n addrServer: %s, porta: %d", inet_ntoa(ServerAddr.sin_addr), htons(ServerAddr.sin_port));
		int sndMSG=sendto(ClientSocket, messaggio, sizeof(messaggio),0, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
		if(sndMSG<0){
			printf("\n\nSendError");
		}
		else{
			char buffer[MAX_LEN];
			printf("\n\nInserisci messaggio da inviare al server:\n");
			scanf(" %[^\n]", buffer);
			strcpy(messaggio, buffer);
			if((sndMSG=sendto(ClientSocket, messaggio, sizeof(messaggio),0, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr)))<0){
				perror("\n\n Send Message Failed\n\n");
				return -1;
			}
			else{
				int SVLen=sizeof(ServerAddr);
				int rcvMSG;
				if((rcvMSG=recvfrom(ClientSocket, messaggio, sizeof(messaggio), 0, (struct sockaddr*)&ServerAddr, &SVLen))<0){
					return -1;
				}
				else{
					printf("\n\nStringa \"%s\" ricevuta dal server nome:%s inidirizzo:%s", messaggio, name, inet_ntoa(ServerAddr.sin_addr));
				}
			}
		}


	}
	else{
		printf("serverHost error");
		system("pause");
	}

	closesocket(ClientSocket);

	return 0;
}
