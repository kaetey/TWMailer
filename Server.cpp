#include <sys/socket.h>
#include <string.h>

#define PORT 8080 //portnumber?
#define BUF 1024

int main(void) {
	//create socket
	int socket = -1;

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

	printf("Waiting for connections...\n");

	//accept connections from client
	int new_socket = -1;
	struct sockaddr_in client_adress;

	if ((new_socket = accept(socket, (struct sockaddr*)&client_adress, &sizeof(struct 											sockaddr_in)) == -1)
	{

	}


	void clientCommunication(void* curr_socket) {
		int* current_socket = (int*)curr_socket;
			std::string welc_msg = "Welcome!\n Please enter your command!\n";
			//send welcome message to client
			if (send(current_socket, welc_msg, strlen(welc_msg), 0) {
				perror("send failed");
					return NULL; //why null??
			}
			else {
				printMenu();
			}

		do {
			//receive data from client
			char buffer[BUF];
				int size;
			if (size = recv(*current_socket, buffer, BUF - 1, 0) == -1) {
				perror("receive failed");
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

			if (send(*current_socket, "OK", 3, 0) == -1)
			{
				perror("send answer failed");
				return NULL;
			}
		} while ()

	}

	void printMenu() {
	}








}