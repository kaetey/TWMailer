#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define BUF 1024
#define PORT 5901


void sendMessage(int create_socket);

void listMessage(int create_socket);

void readMessage(int create_socket);

void deleteMessage(int create_socket);

int main(int argc, char **argv)
{
    int create_socket;
    char buffer[BUF];
    struct sockaddr_in address;
    int size;
    int isQuit;


    if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket error");
        return EXIT_FAILURE;
    }

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;         // IPv4
    address.sin_port = htons(PORT);
    if (argc < 2)
    {
        inet_aton("127.0.0.1", &address.sin_addr);
    }
    else
    {
        inet_aton(argv[1], &address.sin_addr);
    }

    // CREATE A CONNECTION
    if (connect(create_socket,
                (struct sockaddr *)&address,
                sizeof(address)) == -1)
    {
        perror("Connect error - no server available");
        return EXIT_FAILURE;
    }

    // ignore return value of printf
    printf("Connection with server (%s) established\n",
           inet_ntoa(address.sin_addr));

    // RECEIVE DATA
    size = recv(create_socket, buffer, BUF - 1, 0);
    if (size == -1)
    {
        perror("recv error");
    }
    else if (size == 0)
    {
        printf("Server closed remote socket\n"); // ignore error
    }
    else
    {
        buffer[size] = '\0';
        printf("%s", buffer); // ignore error
    }
    printf("SEND: send a message from client to server.\n"
           "LIST: list all messages from a specific user.\n"
           "READ: read a specific message from a user.\n"
           "DEL: del a specific message from a user.\n"
           "QUIT: Disconnect and quit program.\n");
    do
    {
        printf(">> ");
        if (fgets(buffer, BUF, stdin) != NULL) {
            int size = strlen(buffer);
            // remove new-line signs from string at the end
            if (buffer[size - 2] == '\r' && buffer[size - 1] == '\n') {
                size -= 2;
                buffer[size] = 0;
            } else if (buffer[size - 1] == '\n') {
                --size;
                buffer[size] = 0;
            }

            if (strcmp(buffer, "SEND") == 0) {
                sendMessage(create_socket);
            } else if (strcmp(buffer, "LIST") == 0) {
                listMessage(create_socket);
            } else if (strcmp(buffer, "READ") == 0) {
                readMessage(create_socket);
            } else if (strcmp(buffer, "DEL") == 0) {
                deleteMessage(create_socket);
            } else if (strcmp(buffer, "QUIT") == 0) {
                isQuit = 1;
            } else {
                printf("Please enter a valid command:\nSEND--LIST--READ--DEL--QUIT\n");
                continue;
            }
        }

    } while (!isQuit);

    // CLOSES THE DESCRIPTOR
    if (create_socket != -1)
    {
        if (shutdown(create_socket, SHUT_RDWR) == -1)
        {
            // invalid in case the server is gone already
            perror("shutdown create_socket");
        }
        if (close(create_socket) == -1)
        {
            perror("close create_socket");
        }
        create_socket = -1;
    }

    return EXIT_SUCCESS;
}

void deleteMessage(int create_socket) {
    char message[20];
    char msgNumber[5];
    char username[8]; //8 or maybe 9 with \n
    char recvMessage[BUF];

    printf("Enter a username: ");
    if (fgets(username, 8, stdin) != NULL) {
        int size = strlen(username);
        // remove new-line signs from string at the end
        if (username[size - 2] == '\r' && username[size - 1] == '\n') {
            size -= 2;
            username[size] = 0;
        } else if (username[size - 1] == '\n') {
            --size;
            username[size] = 0;
        }
    }else{
        exit(EXIT_FAILURE);
    }
    printf("Enter the message number: ");
    if (fgets(msgNumber, 5, stdin) != NULL) {
        int size = strlen(msgNumber);
        // remove new-line signs from string at the end
        if (msgNumber[size - 2] == '\r' && msgNumber[size - 1] == '\n') {
            size -= 2;
            msgNumber[size] = 0;
        } else if (msgNumber[size - 1] == '\n') {
            --size;
            msgNumber[size] = 0;
        }
    }else{
        exit(EXIT_FAILURE);
    }

    strcat(message, "DEL\n");
    strcat(message, username);
    strcat(message, "\n");
    strcat(message, msgNumber);
    strcat(message, "\n");

    if(send(create_socket, message, strlen(message), 0) == -1) {
        perror("DEL failed to send!");
        exit(EXIT_FAILURE);
    }
    if(recv(create_socket, recvMessage, strlen(recvMessage), 0) == -1){
        perror("DEL failed to receive!");
        exit(EXIT_FAILURE);
    }
    printf(recvMessage);
}

void readMessage(int create_socket) {
    char message[20];
    char msgNumber[5];
    char username[8]; //8 or maybe 9 with \n
    char recvMessage[BUF];

    printf("Enter a username: ");
    if (fgets(username, 8, stdin) != NULL) {
        int size = strlen(username);
        // remove new-line signs from string at the end
        if (username[size - 2] == '\r' && username[size - 1] == '\n') {
            size -= 2;
            username[size] = 0;
        } else if (username[size - 1] == '\n') {
            --size;
            username[size] = 0;
        }
    }else{
        exit(EXIT_FAILURE);
    }
    printf("Enter the message number: ");
    if (fgets(msgNumber, 5, stdin) != NULL) {
        int size = strlen(msgNumber);
        // remove new-line signs from string at the end
        if (msgNumber[size - 2] == '\r' && msgNumber[size - 1] == '\n') {
            size -= 2;
            msgNumber[size] = 0;
        } else if (msgNumber[size - 1] == '\n') {
            --size;
            msgNumber[size] = 0;
        }
    }else{
        exit(EXIT_FAILURE);
    }

    strcat(message, "READ\n");
    strcat(message, username);
    strcat(message, "\n");
    strcat(message, msgNumber);
    strcat(message, "\n");

    if(send(create_socket, message, strlen(message), 0) == -1) {
        perror("READ failed to send!");
        exit(EXIT_FAILURE);
    }
    if(recv(create_socket, recvMessage, strlen(recvMessage), 0) == -1){
        perror("READ failed to receive!");
        exit(EXIT_FAILURE);
    }
    printf(recvMessage);
}

void listMessage(int create_socket) {
    char message[15];
    char username[8]; //8 or maybe 9 with \n
    char recvMessage[BUF];

    printf("Enter a username: ");
    if (fgets(username, 8, stdin) != NULL) {
        int size = strlen(username);
        // remove new-line signs from string at the end
        if (username[size - 2] == '\r' && username[size - 1] == '\n') {
            size -= 2;
            username[size] = 0;
        } else if (username[size - 1] == '\n') {
            --size;
            username[size] = 0;
        }
    }else{
        exit(EXIT_FAILURE);
    }
    strcat(message, "LIST\n");
    strcat(message, username);
    strcat(message, "\n");

    if(send(create_socket, message, strlen(message), 0) == -1) {
        perror("LIST failed to send!");
        exit(EXIT_FAILURE);
    }
    if(recv(create_socket, recvMessage, strlen(recvMessage), 0) == -1){
        perror("LIST failed to receive!");
        exit(EXIT_FAILURE);
    }
    printf(recvMessage);
}

void sendMessage(int create_socket) {
    char message[BUF];
    char sender[8];
    char receiver[8];
    char subject[80];
    char recvMessage[BUF];
    char *c = nullptr;

    printf("Enter a username for the sender: ");
    if(fgets(sender, 8, stdin) != NULL) {
        int size = strlen(sender);
        // remove new-line signs from string at the end
        if (sender[size - 2] == '\r' && sender[size - 1] == '\n') {
            size -= 2;
            sender[size] = 0;
        } else if (sender[size - 1] == '\n') {
            --size;
            sender[size] = 0;
        }
    }else{
        exit(EXIT_FAILURE);
    }
    printf("Enter a username for the receiver: ");
    if (fgets(receiver, 8, stdin) != NULL) {
        int size = strlen(receiver);
        // remove new-line signs from string at the end
        if (receiver[size - 2] == '\r' && receiver[size - 1] == '\n') {
            size -= 2;
            receiver[size] = 0;
        } else if (receiver[size - 1] == '\n') {
            --size;
            receiver[size] = 0;
        }
    }else{
        exit(EXIT_FAILURE);
    }
    printf("Enter the subject of the message: ");
    if (fgets(subject, 80, stdin) != NULL) {
        int size = strlen(subject);
        // remove new-line signs from string at the end
        if (subject[size - 2] == '\r' && subject[size - 1] == '\n') {
            size -= 2;
            subject[size] = 0;
        } else if (subject[size - 1] == '\n') {
            --size;
            subject[size] = 0;
        }
    }else{
        exit(EXIT_FAILURE);
    }
    strcat(message, sender);
    strcat(message, "\n");
    strcat(message, receiver);
    strcat(message, "\n");
    strcat(message, subject);
    strcat(message, "\n");

    printf("Enter the message. Include a dot ('.') in a sentence to exit: ");
    do{
        *c = getchar();
        strcat(message, c);
    }while(*c != '.');
    strcat(message, "\n");

    if(send(create_socket, message, strlen(message), 0) == -1) {
        perror("LIST failed to send!");
        exit(EXIT_FAILURE);
    }
    if(recv(create_socket, recvMessage, strlen(recvMessage), 0) == -1){
        perror("LIST failed to receive!");
        exit(EXIT_FAILURE);
    }
    printf(recvMessage);
}
