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
            //printf("\n\n%d passo: %s",index,stringa);
    	}
    	else{
    	    index++;
    	}
	}
}


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
	struct sockaddr_in ServerAddr;
	struct sockaddr_in ClientAddr;


	//server addr set
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	ServerAddr.sin_port=htons(10990);


	if((ServerSocket=socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP))<0){
		perror("\nSocket creation Error");
		return -1;
	}

	if(bind(ServerSocket, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr))<0){
		perror("\nbind Error");
				return -1;
	}

	while(1){
		//ascolta
		//riceve 123,11.11
		int CLen= sizeof(ClientAddr);
		char msgBuffer[50];
		printf("\nIn attesa di connessioni...\n\n");
		int rcvMSG;
		if((rcvMSG=recvfrom(ServerSocket,msgBuffer, sizeof(msgBuffer), 0, (struct sockaddr*)&ClientAddr, &CLen))<0){
			printf("fatti in culo");
		}
		//getnamebyaddress
		struct hostent *ClientHost;
		int inAddrLen=sizeof(struct in_addr);
		ClientHost=gethostbyaddr((char*)&ClientAddr.sin_addr, sizeof(struct in_addr), AF_INET);
		if(ClientHost != NULL){
			printf("\n\nRicevuti dati dal client nome:%s\nIndirizzo:%s",ClientHost->h_name, inet_ntoa(ClientAddr.sin_addr));
			int rcvMSG=recvfrom(ServerSocket,msgBuffer, sizeof(msgBuffer), 0, (struct sockaddr*)&ClientAddr, &CLen);

			printf("\n\nMessaggio ricevuto dal client: %s", msgBuffer);

			cancellaVocali(msgBuffer, sizeof(msgBuffer));

			int sndMSG;
			if((sndMSG=sendto(ServerSocket, msgBuffer, sizeof(msgBuffer), 0, (struct sockaddr*)&ClientAddr, sizeof(ClientAddr)))<0){
				printf("\n\nSend Error\n\n");
				return -1;
			}
			else{
				printf("\n\nMessaggio inviato: %s\n\n", msgBuffer);
			}
		}
		else{
			printf("\n\nHost Setting Error\n\n");
			return -1;
		}

	}

	return 0;
}
