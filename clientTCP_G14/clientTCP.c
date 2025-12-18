/*
 * clientTCP.c
 *
 *  Created on: 16 dic 2025
 *      Author: Andrea Martilotti
 *      Author: Michele Ritella
 */


#include <stdio.h>
#include <winsock.h>
#include <string.h>

#define porta 10990
#define MAX_LEN 50


typedef struct MSG {
	int valore;
	char testo[MAX_LEN];
} Messaggio;

int main() {

	WSADATA wsaData;
	WORD wVersionRequested;
	wVersionRequested= MAKEWORD(2,2);
	int iResult = WSAStartup(wVersionRequested, &wsaData);
	if (iResult != 0) {
		printf("Error at WSAStartup()\n");
		return -1;
	}

	int ClientSocket;
	struct sockaddr_in server_address;
	struct hostent *hp;
	Messaggio messaggio;


// creazione socket client

	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ClientSocket < 0) {
		perror( "Errore creazione socket client\n" );
		return -1;
	}


// binding

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr( "127.0.0.1" );
	server_address.sin_port = htons(porta);

	// (host aliasing)

	hp = gethostbyname("localhost"); // indirizzo 27015
	if (hp < 0) {
			perror( "Errore localhost\n" );
			return -1;
		}


//connessione

	if (connect(ClientSocket, (struct sockaddr *)&server_address, sizeof(server_address)) <0) {
		perror("Errore connessione");
		return -1;
	}

	strcpy(messaggio.testo, "Hello");
	messaggio.valore = 10;
	if (send(ClientSocket, (char*)&messaggio, sizeof(messaggio),0) <0) {
		perror("Errore send");
		return -1;
	}

	printf("\n\nConnesso al server\n");

char buffer[MAX_LEN];

printf("\n\nInserisci messaggio da inviare al server:\n");
scanf("%[^\n]", buffer);
strcpy(messaggio.testo, buffer);

// messaggio

	messaggio.valore = 10;
	if (send(ClientSocket, (char*)&messaggio, sizeof(messaggio),0) <0) {
		perror("Errore send");
		return -1;
	}



	printf("\n\nMessaggio inviato\n\n");


	Messaggio messRcv;
		if(recv(ClientSocket, (char*)&messRcv, sizeof(messRcv),0)<0){
			perror("Errore invio");
			return -1;
		}
		printf("\n\nMessaggio ricevuto: %s\nCodice: %d\n", messRcv.testo, messRcv.valore);

	system("pause");
closesocket(ClientSocket);
//chiusura connessione

	return 0;
}
