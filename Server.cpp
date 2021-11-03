#include <sys/socket.h>

#define PORT 8080 //portnumber?

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











}