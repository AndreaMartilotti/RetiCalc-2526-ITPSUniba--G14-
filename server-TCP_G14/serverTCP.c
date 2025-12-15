/*
 * serverTCP.c
 *
 *  Created on: 15 dic 2025
 *      Author: Andrea Martilotti
 *      Author: Michele Ritella
 *      prova
 *      prova 9
 */

#include <stdio.h>
#include <string.h>
#include <winsock.h>
#define MAX_LEN 50


typedef struct MSG{
	int value;
	char msg[MAX_LEN];
};

int main (){
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

	ServerSocket=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in server;

	if(ServerSocket<0){
		perror("\n\nSocket Error\n\n");
		return -1;
	}

	//IMPOSTAZIONI SOCKADDR_IN
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1"); //settaggio ip fisso in fase succcessiva
	server.sin_port = htons(0); //messa in assegnazione automatica, eventuale settaggio in fase successiva

	//BINDING DEI SETTINGS ALLA SOCKET ServerSocket
	if(bind(ServerSocket, (struct sockaddr*)&server, sizeof(server))<0){
		perror("\n\nBinding error\n\n");
		return -1;
	}


	//LISTENING

	//imposto numero massimo di connessioni permesse in coda
	int n=5;

	if(listen(ServerSocket, n)<0){
		perror("Listen Error");
		return -1;
	}

	printf("\n\nServer attivo all'indirizzo: %s e porta: %d\n\n", inet_ntoa(server.sin_addr), ntohs(server.sin_port));
	int e;
	scanf("%d", &e);

	printf("\n\nIn attesa di nuove connessioni...\n\n");

	//COSTRUZIONE SOCKET CLIENT
	struct sockaddr_in clientAdd;
	int clientSocket;
	int clientLen;

	while(1){
		clientLen=sizeof(clientAdd);
		if((clientSocket=accept(ServerSocket, (struct sockaddr *)&clientAdd, &clientLen))<0){
			perror("\n\nAccettazione fallita\n\n");
			closesocket(ServerSocket);
			return -1;
		}

		MSG messaggio;

		if(recv(clientSocket, (char *)&messaggio, sizeof(messaggio.message), 0)<0){
			perror("\n\nMSG error\n\n");
			return -1;
		}
	}


	//RECIVE

	return 0;
}
