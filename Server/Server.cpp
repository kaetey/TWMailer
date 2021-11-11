#include <sys/socket.h>
#include <signal.h>
#include <string>
#include <sstream>

#define PORT 5901 //portnumber?
#define BUF 1024 //what does 1024 mean?

int socket, new_socket = -1;
int abortRequested = 0;

int main(void) {
	//signal handler
	if (signal(SIGINT, signalHandler) == SIG_ERR)
	{
		perror("signal can not be registered");
		return EXIT_FAILURE;
	}

	//create socket
	if (socket = socket(AF_INET, SOCK_STREAM, 0) == -1) {
		perror("Socket error");
		return EXIT_FAILURE;

	}

	//set socket options
	int optVal = 1;

	if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal)) == -1) {
		perror("set socket options - reuseAddr error");
		return EXIT_FAILURE;
	}
	if (setsockopt(socket, SOL_SOCKET, SO_REUSEPORT, &optVal, sizeof(optVal)) == -1) {
		perror("set socket options - reusePort error");
		return EXIT_FAILURE;
	}

	//assign an adress to socket
	struct sockaddr_in server_adress;
	memset(&address, 0, sizeof(server_adress)); //empty buffer
	server_adress.sin_family = AF_INET;
	server_adress.sin_port = htons(PORT);
	server_adress.sin_addr.s_addr = INADDR_ANY;

	if (bind(socket, (struct sockaddr*)&server_adress, sizeof(server_adress)) == -1) {
		perror("bind failed");
		return EXIT_FAILURE;
	}

	//listen to pending connections from client
	if (listen(socket, 5) == -1) {
		perror("listen failed");
		return EXIT_FAILURE;
	}

	while (!abortRequested) {
		printf("Waiting for connections...\n");

		//accept connections from client
		struct sockaddr_in client_address;

		if ((new_socket = accept(socket, (struct sockaddr*)&client_address, &sizeof(struct sockaddr_in)) == -1){
			if(abortRequested) perror("accept error after aborted");
			else perror("accept error");
			break;
		}
		
		//start client
		printf("Client connected from %s:%d...\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port)); //ignore error handling
		clientCommunication(&new_socket);
		new_socket = -1; //why?
	}

	// free the descriptor
	if (socket != -1)
	{
		if (shutdown(socket, SHUT_RDWR) == -1)
		{
			perror("shutdown socket");
		}
		if (close(socket) == -1)
		{
			perror("close socket");
		}
		socket = -1;
	}

	return EXIT_SUCCESS;
}

void *clientCommunication(void* curr_socket) { //warum eine stern vor methodenname?
	char buffer[BUF];
	int* current_socket = (int*)curr_socket;
	std::string welc_msg = "Welcome!\r\n"; //geht auch mit buffer

	//send welcome message to client
	if (send(current_socket, welc_msg, strlen(welc_msg), 0) == -1) {
		perror("send failed");
		return NULL; //why null??
	}
	//send menu? or menu im client?

	do {
		//receive data from client
		int size;
		if (size = recv(*current_socket, buffer, BUF - 1, 0) == -1) {
			if (abortRequested) perror("receive failed after aborted");
			else perror("receive failed");
			break;
		}
		else if (size == 0) {
			printf("Client closed remote socket\n"); // ignore error
			break;
		}

		// remove ugly debug message, because of the sent newline of client
		if (buffer[size - 2] == '\r' && buffer[size - 1] == '\n') size -= 2;
		else if (buffer[size - 1] == '\n') size--;

		buffer[size] = '\0';
		printf("Message received: %s\n", buffer); // ignore error

		mailHandler(buffer);

		if (send(*current_socket, "OK", 3, 0) == -1)
		{
			perror("send answer failed");
			return NULL;
		}
	} while (strcmp(buffer, "QUIT") != 0 && !abortRequested);

	// closes/frees the descriptor if not already
	if (*current_socket != -1)
	{
		if (shutdown(*current_socket, SHUT_RDWR) == -1)
		{
			perror("shutdown new_socket");
		}
		if (close(*current_socket) == -1)
		{
			perror("close new_socket");
		}
		*current_socket = -1;
	}

	return NULL; //why return?
}

void signalHandler(int signal) {
	if (signal == SIGINT) {
		printf("abort Requested... "); // ignore error
		abortRequested = 1;

		//free descriptors
		if (new_socket != -1) {
			if (shutdown(new_socket, SHUT_RDWR) == -1) {
				perror("shutdown new_socket");
			}
			if (close(new_socket) == -1) {
				perror("close new_socket");
			}
			new_socket = -1;
		}
		if (socket != -1) {
			if (shutdown(socket, SHUT_RDWR) == -1) {
				perror("shutdown create_socket");
			}
			if (close(socket) == -1) {
				perror("close create_socket");
			}
			socket = -1;
		}
	}
	else {
		exit(signal); //EXIT_SUCCESS?
	}
}

void mailHandler(char buffer[]) {

	std::istringstream message(buffer);
	std::string command;
	getline(message, command);

	if (strcmp(buffer, "SEND") == 0) {
		sendMessage();
	}
	else if (strcmp(buffer, "LIST") == 0) {
		listMessage();
	}
	else if (strcmp(buffer, "READ") == 0) {
		readMessage();
	}
	else if (strcmp(buffer, "DEL") == 0) {
		deleteMessage();
	}
}

void sendMessage() {

}

void listMessage() {

}

void readMessage() {

}

void deleteMessage() {

}