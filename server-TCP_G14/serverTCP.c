/*
 * serverTCP.c
 *
 *  Created on: 15 dic 2025
 *      Author: Andrea Martilotti
 *      Author: Michele Ritella
 */

#include <stdio.h>
#include <string.h>
#include <winsock.h>
#define MAX_LEN 50


typedef struct _MSG{
	int value;
	char msg[MAX_LEN];
}messaggio;




void cancellaVocali(char* str, int MAX_LENG){
	int index=0;
	//controllo vocale
	while(index<MAX_LENG){
    	char stringa=tolower(str[index]);
    	if(stringa=='a'||stringa=='e'||stringa=='i'||stringa=='o'||stringa=='u'){
    		//elimina carattere
    		for(int i=index;i<MAX_LENG;i++){
    			str[i]=str[i+1];
    		}
    		printf("\n\npost eliminazione: %s", str);
            //printf("\n\n%d passo: %s",index,stringa);
    	}
    	else{
    	    index++;
    	    printf("\npassato\n");
    	}
	}
}



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
	server.sin_port = htons(27015); //messa in assegnazione automatica, eventuale settaggio in fase successiva

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
		printf("Connessione accettata da IP: %s, Porta: %d\n", inet_ntoa(clientAdd.sin_addr), ntohs(clientAdd.sin_port));

		messaggio messaggio;

		if(recv(clientSocket, (char *)&messaggio, sizeof(messaggio.msg), 0)<0){
			perror("\n\nMSG recive error\n\n");
			return -1;
		}
		else{
			// rimuovi vocali
			printf("\n\nMessaggio ricevuto da client %s : %s", inet_ntoa(clientAdd.sin_addr), messaggio.msg);
			int strleng=strlen(messaggio.msg);
			cancellaVocali(messaggio.msg, strleng);

			//sendtoclient
			if(send(clientSocket, (char *)&messaggio, sizeof(messaggio.msg), 0)<0){
				perror("\n\nMSG send error\n\n");
				return -1;
			}
			printf("\n\nMessaggio inviato\n\n");

		}

	}


	//RECIVE

	return 0;
}
