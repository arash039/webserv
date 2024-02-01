#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <string.h>

#define MAX_CLIENTS 100

int main() {
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr;
    pollfd fds[MAX_CLIENTS];
    int nfds = 1, new_fd;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(listen_fd, 10);

    fds[0].fd = listen_fd;
    fds[0].events = POLLIN;

    while (1) {
        int n = poll(fds, nfds, -1);
        if (n == -1) {
            perror("poll");
            return 1;
        }

        for (int i = 0; i < nfds; i++) {
            if (fds[i].revents == 0)
                continue;

            if (fds[i].fd == listen_fd) {
                new_fd = accept(listen_fd, NULL, NULL);
                fds[nfds].fd = new_fd;
                fds[nfds].events = POLLIN;
                nfds++;
            } else {
                char buffer[1024];
                int len = read(fds[i].fd, buffer, sizeof(buffer) - 1);
                if (len > 0) {
                    buffer[len] = '\0';
                    std::cout << "Received: " << buffer << std::endl;

                    // Open the HTML file
                    std::ifstream html_data("index.html");

                    if (!html_data) {
                        std::cerr << "Could not open file\n";
                        return 1;
                    }

                    // Read the file into memory
                    std::stringstream buffer;
                    buffer << html_data.rdbuf();
                    std::string response_data = buffer.str();

                    // Prepare the HTTP response
                    std::string httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(response_data.size()) + "\r\n\r\n" + response_data;
                    write(fds[i].fd, httpResponse.c_str(), httpResponse.size());
                } else {
                    close(fds[i].fd);
                    fds[i] = fds[nfds - 1];
                    nfds--;
                }
            }
        }
    }

    return 0;
}
