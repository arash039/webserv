#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>

constexpr int MAX_CLIENTS = 100;

int main() {
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr{};
    std::vector<pollfd> fds(MAX_CLIENTS);
    int nfds = 1, new_fd;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(listen_fd, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr));
    listen(listen_fd, 10);

    fds[0].fd = listen_fd;
    fds[0].events = POLLIN;

    while (true) {
        int n = poll(fds.data(), nfds, 1);
        if (n == -1) {
            std::cerr << "poll error\n";
            return 1;
        }

        for (int i = 0; i < nfds; i++) {
            if (fds[i].revents == 0)
                continue;

            if (fds[i].fd == listen_fd) {
                new_fd = accept(listen_fd, nullptr, nullptr);
                fds[nfds].fd = new_fd;
                fds[nfds].events = POLLIN;
                nfds++;
            } else {
                char buffer[1024];
                int len = read(fds[i].fd, buffer, sizeof(buffer) - 1);
                if (len > 0) {
                    buffer[len] = '\0';
                    std::cout << "Received: " << buffer << "\n";
                    const char httpResponse[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello world!";
                    write(fds[i].fd, httpResponse, sizeof(httpResponse));
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
