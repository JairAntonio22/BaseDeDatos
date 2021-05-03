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
#define SERVER_LOGIN_REPLY_SIZE 8
// Puerto de HTTP
#define PORT 80
// Input buffer size
#define BUFFER_SIZE 256
#define USERNAME_BUFFER_SIZE 30
#define PASSWORD_BUFFER_SIZE 30
#define LOGIN_MAX_TRIES 3

#define OK_INIT 0
#define OK_LOGIN 0
#define OK_REQUEST_SENT 0
#define OK_REPLY_RECEIVED 0
#define OK_RESULT_TABLE_PARSED 0
#define ERROR_SOCKET_NOT_CREATED 1
#define ERROR_SERVER_NOT_CONNECTED 2
#define ERROR_REQUEST_NOT_SENT 3
#define ERROR_REPLY_NOT_RECEIVED 4
#define ERROR_LOGIN_MAX_TRIES 5
#define ERROR_RESULT_TABLE_ROW_NOT_PARSED 6
#define ERROR_RESULT_TABLE_COL_NOT_PARSED 7

// #define DEFAULT -1
// #define QUIT 0
// #define SELECT 1
// #define INSERT 2
// #define JOIN 3
// #define ERROR_NEW_SOCKET -1
// #define BUFFER_SIZE 1024

typedef enum {false, true} bool;

int init();
int send_request();
int receive_reply();
void print_reply();
int print_result_table();
void finish();
int login();
bool logout();
void insert_db();
void select_db();
void join_db();

// Server socket descriptor
int socket_desc;
// Server info struct
struct sockaddr_in server;
// Reply buffer to receive data from server
char server_reply[SERVER_REPLY_SIZE];

char username[USERNAME_BUFFER_SIZE];
char password[PASSWORD_BUFFER_SIZE];
// Input buffer
char buffer[BUFFER_SIZE];

// int choice = DEFAULT;
// char** username;
// char** password;
// char** buffer;
// bool login_status;

int main(void) 
{
    // Login
    switch (login())
    {
    case ERROR_REQUEST_NOT_SENT:
        printf("Could not send request to server\n");
        break;
    case ERROR_REPLY_NOT_RECEIVED:
        printf("Could not receive reply from server\n");
        break;
    case ERROR_LOGIN_MAX_TRIES:
        printf("You have used all attempts. Try again later.\n");
        break;
    case OK_LOGIN:
        printf("Login successful\n");
    default:
        break;
    }

    // 1. Init: create socket and connect to server
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

    // 2. Send request to server
    if (send_request() == ERROR_REQUEST_NOT_SENT)
    {
        printf("Send failed\n");
        return 1;
    }

    printf("Data sent\n");

    // 4. Receive reply from server
    if (receive_reply() == ERROR_REPLY_NOT_RECEIVED)
    {
        printf("Reply not received");
        return 1;
    }

    printf("Reply received\n");

    // 5. Print reply
    // print_reply();
    switch (print_result_table())
    {
    case ERROR_RESULT_TABLE_ROW_NOT_PARSED:
        printf("Rows could not be retreived\n");
        break;
    case ERROR_RESULT_TABLE_COL_NOT_PARSED:
        printf("Columns could not be retreived\n");
        break;
    case OK_RESULT_TABLE_PARSED:
        printf("Table parse successful\n");
        break;
    default:
        break;
    }
    
    // 6. Close socket
    close(socket_desc);
    printf("Socket closed\n");
    return 0;
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

int send_request()
{
    // Message buffer to send data to server
    char* message = "GET / HTTP/1.1\r\n\r\n";

    if (send(socket_desc, message, strlen(message), 0) < 0)
        return ERROR_REQUEST_NOT_SENT;

    return OK_REQUEST_SENT;
}

int receive_reply()
{
    if (recv(socket_desc, server_reply, SERVER_REPLY_SIZE, 0) < 0)
        return ERROR_REPLY_NOT_RECEIVED;

    return OK_REPLY_RECEIVED;
}

void print_reply()
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

int print_result_table()
{
    // Reply
    // PRUEBA: asi debe lucir el mensaje de respuesta del servidor
    char* reply = "1,2,3\n4,5,6\n7,8,9";
    // Row and column delimiters
    char* delim_row = "\n";
    char* delim_col = ",";
    // Separator for final print
    char* separator = "\t";

    // Pointers to receive the beginning of each row and column
    char *row, *col;
    // Pointers to maintain context between strtok_r() simultaenous calls
    char *saveptr1, *saveptr2;

    // Copy reply to buffer to avoid reference issues after call to strtok_r()
    char* copy = malloc(strlen(reply) + 1);
    strcpy(copy, reply);

    // Parse all rows and all columns per row from message
    // First call to strok_r for all rows: point to message, next calls will point to NULL
    row = strtok_r(copy, delim_row, &saveptr1);

    // Error: token not found
    if (row == NULL)
        return ERROR_RESULT_TABLE_ROW_NOT_PARSED;

    // First call to strok_r for all cols in current row: point to row, next calls will point to NULL
    col = strtok_r(row, delim_col, &saveptr2);

    // Error: token not found
    if (col == NULL)
        return ERROR_RESULT_TABLE_COL_NOT_PARSED;
    
    // Print first col
    printf("%s%s", col, separator);
    
    // Print all remaining cols in current row
    while ((col = strtok_r(NULL, delim_col, &saveptr2)) != NULL)
        printf("%s%s", col, separator);
    printf("\n");

    // Parse remaining rows in message
    while ((row = strtok_r(NULL, delim_row, &saveptr1)) != NULL)
    {   
        col = strtok_r(row, delim_col, &saveptr2);
        if (col == NULL)
            return ERROR_RESULT_TABLE_COL_NOT_PARSED;

        // Print first col
        printf("%s%s", col, separator);

        // Print all remaining cols in current row
        while ((col = strtok_r(NULL, delim_col, &saveptr2)) != NULL)
            printf("%s%s", col, separator);
        printf("\n");
    }

    free(copy);
    return OK_RESULT_TABLE_PARSED;

}

void finish()
{

}

int login()
{
    // Set max number of login attempts
    int max_tries = LOGIN_MAX_TRIES;
    // Buffer to receive reply from server
    // PRUEBA: el valor es asignado para probar funcionalidad correcta
    // Cambiar success o failure segun se quiera
    char* reply = "success";

    printf("Please login to continue\n");

    // Try login for max number of attempts
    do
    {
        // Read username from console
        printf("Username: ");
        fgets(username, USERNAME_BUFFER_SIZE, stdin);
        username[strlen(username) - 1] = '\0';
        
        // Read password from console
        printf("Password: ");
        fgets(password, PASSWORD_BUFFER_SIZE, stdin);
        password[strlen(password) - 1] = '\0';

        // Join username and password in a comma-separated message to send to server
        char* message = malloc(strlen(username) + strlen(password) + 2);
        strcpy(message, username);
        strcat(message, ",");
        strcat(message, password);

        // PRUEBA: verificar que el mensaje este correctamente formateado
        printf("|%s|\n", message);

        // Send request to server
        // if (send(socket_desc, message, strlen(message), 0) < 0)
        //     return ERROR_REQUEST_NOT_SENT;

        // Receive reply from server
        // if (recv(socket_desc, reply, SERVER_REPLY_SIZE, 0) < 0)
        //     return ERROR_REPLY_NOT_RECEIVED;

        // Check if login was successful
        if (strcmp(reply, "success") == 0)
            return OK_LOGIN; // Success
        
        // Failure: try again
        printf("\nUser and password do not match. Try again.\n");
        free(message);
        max_tries--;
    } while (max_tries > 0);
    
    // User exceeded max number of attempts
    return ERROR_LOGIN_MAX_TRIES;
}

void insert_db()
{

}

void select_db()
{

}

void join_db()
{

}