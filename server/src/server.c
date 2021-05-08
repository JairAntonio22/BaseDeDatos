#include "db.h"
//#include "../db/src/table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#define MAX 80
#define PORT 3000
#define SA struct sockaddr

// Function designed for chat between client and server.
void func(int sockfd)
{
	char buff[MAX];
	int n;
	// infinite loop for chat
	for (;;) {
		bzero(buff, MAX);

		// read the message from client and copy it in buffer
		read(sockfd, buff, sizeof(buff));
		// print buffer which contains the client contents
		printf("From client: %s\n", buff);
		char cadenaQuery[sizeof(buff)];
		char cadenaTabla[sizeof(buff)];
		int esComa = 0, sizeQuery = 0, sizeTabla = 0;
		for(int i = 0; i < sizeof(buff); i++){
			if(buff[i] == ','){
				esComa++;
			}
			if(esComa == 0){
				cadenaQuery[i] = buff[i];
				sizeQuery++;
			}
			if(esComa == 1 && buff[i] != ','){
				cadenaTabla[sizeTabla] = buff[i];
				sizeTabla++;
			}
			if(buff[i+1] == NULL){
				i = sizeof(buff);
			}
		}
		char* query = malloc(sizeQuery-1);
		char* tabla = malloc(sizeTabla);
		strcat(query, cadenaQuery);
		strcat(tabla, cadenaTabla);
		printf("Query = %s\n", query, sizeQuery);
		printf("Tabla = %s\n", tabla);
		
		DB *db = load_db("MyDB.txt");
		char *cols[] = {"Nombre"};
        char *where[] = {"Sexo", "Hombre"};
        Table *table = select_db(db, "Personas", 1, cols, where);
        print_table(table);
		
		if(query == "select_all"){
			printf("SIIIIIIII");
		}else{
			printf("NOOOO");
		}
		printf("To client : ");
		bzero(buff, MAX);
		n = 0;
		// copy server message in the buffer
		while ((buff[n++] = getchar()) != '\n')
			;

		// and send that buffer to client
		write(sockfd, buff, sizeof(buff));

		// if msg contains "Exit" then server exit and chat ended.
		if (strncmp("exit", buff, 4) == 0) {
			printf("Server Exit...\n");
			break;
		}
	}
}

// Driver function
int main()
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli);

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("server acccept failed...\n");
		exit(0);
	}
	else
		printf("server acccept the client...\n");

	// Function for chatting between client and server
	func(connfd);

	// After chatting close the socket
	close(sockfd);
}

