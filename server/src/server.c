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
	// infinite loop for chat
	for (;;) {
		bzero(buff, MAX);

		// read the message from client and copy it in buffer
		read(sockfd, buff, sizeof(buff));
		// print buffer which contains the client contents
		printf("From client: %s\n", buff);
		if (strcmp(buff, "quit") == 0) {
			printf("Server Exit...\n");
			char * mensaje = "Log out success";
			write(sockfd, mensaje, strlen(mensaje));
			break;
		}
		char ** str_arr = calloc(sizeof(char*), 500);
		// Extract the first token
		char * token = strtok(buff, ",");
		// loop through the string to extract all other tokens
		int contador = 0;
		while( token != NULL ) {
			str_arr[contador] = strdup(token); //printing each token
			token = strtok(NULL, ",");
			contador++;
		}
		DB *db = load_db("MyDB.txt");
		//Insert
		if(strcmp(str_arr[0], "insert") == 0){
			char ** values = calloc(sizeof(char*), contador-2);
			for(int i = 0; i < contador-2; i++){
				values[i] = str_arr[i+2];
			}
			Error error = insert_db(db, str_arr[1], values);
        	char * mensaje = "";
			switch (error)
			{
				case SuccessOperation:{
					Error guardar = save_db(db);
					if(guardar == SuccessOperation){
						printf("%s se actualizo correctamente", str_arr[1]);
						mensaje = "Se realizo el insert de manera exitosa";
					}else{
						printf("%s no se pudo actualizar", str_arr[1]);
						mensaje = "Hubo un problema con al insertar los datos";
					}
					write(sockfd, mensaje, strlen(mensaje));
				break;
				}

				case NullPtrError:
					mensaje = "ERROR: la base de datos es nula";
					write(sockfd, mensaje, strlen(mensaje));
				break;

				case TableNotFound:
					mensaje = "ERROR: No se pudo encontrar la tabla ";
					write(sockfd, mensaje, strlen(mensaje));
				break;
				
				default:
				printf("ERROR FATAL\n");
				break;
			}
		}
		//Select All
		if(strcmp(str_arr[0], "select_all") == 0){
			Table *table = select_db(db, str_arr[1], 1, NULL, NULL, select_all);
        	if(table != NULL){
			//print_table(table);
			//printf("%s\n", encode_table(table));
			write(sockfd, encode_table(table), strlen(encode_table(table)));
			delete_table(table);
			}else{
				char * mensaje = "ERROR: Verifica bien los datos";
				write(sockfd, mensaje, strlen(mensaje));
			}
		}
		//Select All where
		if(strcmp(str_arr[0], "select_all_where") == 0){
			char *where[2];
            where[0] = strdup(str_arr[2]);
            where[1] = strdup(str_arr[3]);

			Table *table = select_db(db, str_arr[1], 0, NULL, where,select_where);
			if(table != NULL){
				//print_table(table);
				//printf("%s\n", encode_table(table));
				write(sockfd, encode_table(table), strlen(encode_table(table)));
				delete_table(table);
				free(where[0]);
				free(where[1]);
			}else{
				char * mensaje = "ERROR: Verifica bien los datos";
				write(sockfd, mensaje, strlen(mensaje));
			}
		}
		//Select all cols
		if(strcmp(str_arr[0], "select_all_cols") == 0){
			char ** cols = calloc(sizeof(char*), contador-2);
			for(int i = 0; i < contador-2; i++){
				cols[i] = str_arr[i+2];
			}
			Table *table = select_db(db, str_arr[1], contador-2,cols,NULL,select_cols);
 			if(table != NULL){
				//print_table(table);
				//printf("%s\n", encode_table(table));
				write(sockfd, encode_table(table), strlen(encode_table(table)));
			}else{
				char * mensaje = "ERROR: Verifica bien los datos";
				write(sockfd, mensaje, strlen(mensaje));
			}
			
		}
		//Select cols where
		if(strcmp(str_arr[0], "select_cols_where") == 0){
			char ** cols = calloc(sizeof(char*), contador-4);
			for(int i = 0; i < contador-4; i++){
				cols[i] = str_arr[i+4];
			}
			char *where[2];
            where[0] = strdup(str_arr[2]);
            where[1] = strdup(str_arr[3]);
			Table *table = select_db(db, str_arr[1], contador-4,cols,where,select_cols_where);
 			if(table != NULL){
				//print_table(table);
				//printf("%s\n", encode_table(table));
				write(sockfd, encode_table(table), strlen(encode_table(table)));
				free(where[0]);
				free(where[1]);
			}else{
				char * mensaje = "ERROR: Verifica bien los datos";
				write(sockfd, mensaje, strlen(mensaje));
			}
		}
		//Join
		if(strcmp(str_arr[0], "join") == 0){
			char *cols[] = {str_arr[3], str_arr[4]};
			Table *table = join_db(db, str_arr[1], str_arr[2], cols);
			if(table != NULL){
				//print_table(table);
				//printf("%s\n", encode_table(table));
				write(sockfd, encode_table(table), strlen(encode_table(table)));
				delete_table(table);
			}else{
				char * mensaje = "ERROR: Verifica bien los datos";
				write(sockfd, mensaje, strlen(mensaje));
			}
		}
		bzero(buff, MAX);
		printf("------------------------------\n");
		// if msg contains "Exit" then server exit and chat ended.
		
	}
}

// Driver function
int main()
{
	int sockfd, connfd;
    unsigned int len;
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

