#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(5000);
	// Connect to the server at the specified address and port number.
    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    char buffer[1024];
	// Read a line of text from the standard input into the buffer.
    std::cin.getline(buffer, sizeof(buffer));
    write(sockfd, buffer, strlen(buffer) + 1);

    close(sockfd);
    return 0;
}
