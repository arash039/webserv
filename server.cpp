#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

int main() {
	// Create a socket and return a file descriptor that can be used 
	// in later system calls. AF_INET specifies that it's an IPv4 protocol,
	// SOCK_STREAM specifies that it's a TCP protocol.
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	// Declare a variable servaddr of type sockaddr_in to hold the server address.
    sockaddr_in servaddr;
	// Specify the address family, which is IPv4.
    servaddr.sin_family = AF_INET;
	// Specify the IP address. INADDR_ANY means any interface on the system.
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	// Specify the port number. htons() ensures that the number is stored in memory in network byte order.
    servaddr.sin_port = htons(5000);
	// Bind the socket to the address and port number specified in servaddr.
    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	// Mark the socket as passive, i.e., it will be used to accept incoming connection requests. 
	// The second argument specifies the maximum length of the queue of pending connections.
    listen(sockfd, 10);

    while (true) {
        sockaddr_in clientaddr;
        socklen_t clientaddrlen = sizeof(clientaddr);
		// Accept a new connection. The client address is stored in clientaddr.
        int clientfd = accept(sockfd, (struct sockaddr*)&clientaddr, &clientaddrlen);

        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        read(clientfd, buffer, sizeof(buffer));
        std::cout << "Received: " << buffer << std::endl;

		std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>Hello, World!</h1>\r\n";
        write(clientfd, response.c_str(), response.size());
        close(clientfd);
    }

    close(sockfd);
    return 0;
}
