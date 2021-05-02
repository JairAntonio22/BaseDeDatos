#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
// PRUEBA: IP de Google
#define SERVER_IP "142.250.69.14"
#define SERVER_REPLY_SIZE 2000
// Puerto de HTTP
#define PORT 80

#define OK_INIT 0
#define OK_REPLY_RECEIVED 0
#define ERROR_SOCKET_NOT_CREATED 1
#define ERROR_SERVER_NOT_CONNECTED 2
#define ERROR_DATA_NOT_SENT 3
#define ERROR_REPLY_NOT_RECEIVED 4

// #define DEFAULT -1
// #define QUIT 0
// #define SELECT 1
// #define INSERT 2
// #define JOIN 3
// #define ERROR_NEW_SOCKET -1
// #define BUFFER_SIZE 1024

//typedef enum {false, true} bool;

int init();
int receive_reply();
void print_result_table();
// void close();
// bool login(char* username, char* password);
// bool logout();
// void insert();
// void select();
// void join();

// Server socket descriptor
int socket_desc;
// Server info struct
struct sockaddr_in server;
// Message buffer to send data to server
char* message;
// Reply buffer to receive data from server
char server_reply[SERVER_REPLY_SIZE];

// int choice = DEFAULT;
// char** username;
// char** password;
// char** buffer;
// bool login_status;

int main(void) 
{
    switch (init())
    {
    case ERROR_SOCKET_NOT_CREATED:
        printf("Could not create socket\n");
        return 1;
        break;
    case ERROR_SERVER_NOT_CONNECTED:
        printf("Could not connect to server\n");
        return 1;
        break;
    default:
        printf("Connected\n");
        break;
    }

    // 4. Send data
    message = " GET / HTTP/1.1\r\n\r\n";

    if (send(socket_desc, message, strlen(message), 0) < 0)
    {
        printf("Send failed\n");
        return 1;
    }

    printf("Data sent\n");

    if (receive_reply() == ERROR_REPLY_NOT_RECEIVED)
    {
        printf("Reply not received");
        return 1;
    }

    printf("Reply received\n");
    print_result_table();
    
    close(socket_desc);
    return 0;

    // read()

    // write()

    // close()



    // do
    // {
    //     login_status = login(username, password);
    // } while (login_status == false);

    // printf("Type your query followed by a backspace (%d characters max.)\n", BUFFER_SIZE);
    // printf("Type q to quit\n");

    // do 
    // {
    //     switch (choice)
    //     {
    //     case SELECT:
    //         select();
    //         break;
    //     case INSERT:
    //         insert();
    //         break;
    //     case JOIN:
    //         join();
    //         break;
    //     case QUIT:
    //         break;
    //     default:
    //         break;
    //     }
    // }
    // while(choice != QUIT);

    
}

int init()
{
    // 1. Create new socket using IPv4 and TCP/IP
    if ((socket_desc = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return ERROR_SOCKET_NOT_CREATED;

    // 2. Define server values: address family, address and port
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr(SERVER_IP);

    // 3. Connect socket to server
    if (connect(socket_desc, (struct sockaddr*) &server, sizeof(server)) < 0)
        return ERROR_SERVER_NOT_CONNECTED;

    return OK_INIT;
}

int receive_reply()
{
    if (recv(socket_desc, server_reply, SERVER_REPLY_SIZE, 0) < 0)
    {
        printf("recv failed");
        return ERROR_REPLY_NOT_RECEIVED;
    }

    printf("Reply received\n");
}

void print_result_table()
{
    //FILE* fptr;
    //fopen("reply.txt", "w");

    for (int i = 0; i < SERVER_REPLY_SIZE; i++)
    {
        //fprintf(fptr, "%c", server_reply[i]);
        printf("%c", server_reply[i]);
    }
        
    printf("\n");
    //fclose(fptr);
}

// bool login(char* username, char* password)
// {
//     return true;
// }

// void insert()
// {

// }

// void select()
// {

// }

// void join()
// {

// }