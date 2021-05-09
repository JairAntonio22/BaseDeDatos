#include "db.h"
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
		char ** str_arr = calloc(sizeof(char*), 500);
		// Extract the first token
		char * token = strtok(buff, ",");
		// loop through the string to extract all other tokens
		int i = 0, contador = 0;
		while( token != NULL ) {
			str_arr[i] = strdup(token); //printing each token
			token = strtok(NULL, ",");
			i++;
		}
		for(int i = 0; str_arr[i] != NULL; i++){
			printf("%s\n", str_arr[i]);
			contador++;
		}
		printf("%i\n", contador);
		DB *db = load_db("MyDB.txt");
		//Insert
		if(strcmp(str_arr[0], "insert") == 0){
			char ** values = calloc(sizeof(char*), contador-2);
			for(int i = 0; i < contador-2; i++){
				values[i] = str_arr[i+2];
			}
			Error *error = insert_db(db, str_arr[1], values);
        	printf("%i\n", error);
		}
		//Select All
		if(strcmp(str_arr[0], "select_all") == 0){
			Table *table = select_db(db, str_arr[1], 1, NULL, NULL, select_all);
        	print_table(table);
			printf("%s\n", encode_table(table));
			//write(sockfd, encode_table(table), sizeof(encode_table(table)));
			delete_table(table);
		}
		//Select All where
		if(strcmp(str_arr[0], "select_all_where") == 0){
			char * where[] = {str_arr[2], str_arr[3]};
			Table *table = select_db(db, str_arr[1], NULL, NULL, where,select_where);
        	print_table(table);
			printf("%s\n", encode_table(table));
			//write(sockfd, encode_table(table), sizeof(encode_table(table)));
			delete_table(table);
		}
		//Select all cols
		if(strcmp(str_arr[0], "select_all_cols") == 0){
			char ** cols = calloc(sizeof(char*), contador-2);
			for(int i = 0; i < contador-2; i++){
				cols[i] = str_arr[i+2];
			}
			Table *table = select_db(db, str_arr[1], 1,NULL,cols,select_cols);
 			print_table(table);
			printf("%s\n", encode_table(table));
		}
		//Select cols where
		if(strcmp(str_arr[0], "select_cols_where") == 0){
			char ** cols = calloc(sizeof(char*), contador-2);
			for(int i = 0; i < contador-2; i++){
				cols[i] = str_arr[i+2];
			}
			Table *table = select_db(db, str_arr[1], 1,NULL,cols,select_cols_where);
 			print_table(table);
			printf("%s\n", encode_table(table));
		}
		//Join
		if(strcmp(str_arr[0], "join") == 0){
			char *cols[] = {str_arr[3], str_arr[4]};
			Table *table = join_db(db, str_arr[1], str_arr[2], cols);
        	print_table(table);
			printf("%s\n", encode_table(table));
			delete_table(table);
		}
		printf("To client : ");
		bzero(buff, MAX);
		n = 0;
		// copy server message in the buffer
		while ((buff[n++] = getchar()) != '\n');
		
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

