#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// Function to read an entire file into a string
std::string readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

int main() {
    // Create a socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Bind the socket to a specific IP and port
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));

    // Listen for incoming connections
    listen(server_fd, 3);

    while (true) {
        // Accept a new connection
        int new_socket = accept(server_fd, NULL, NULL);

        // Read the file data
        std::string fileData = readFile("index.html");

        // Create the HTTP response
        std::ostringstream response;
        response << "HTTP/1.1 200 OK\r\n"
                 << "Content-Type: application/octet-stream\r\n"
                 << "Content-Length: " << fileData.size() << "\r\n"
                 << "Connection: close\r\n"
                 << "\r\n"
                 << fileData;

        // Send the response
        std::string responseStr = response.str();
        send(new_socket, responseStr.c_str(), responseStr.size(), 0);

        // Close the connection
        close(new_socket);
    }

    return 0;
}
