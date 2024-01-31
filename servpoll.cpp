#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>

#define MAX_CLIENTS 100

int main() {
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    struct pollfd fds[MAX_CLIENTS];
    int nfds = 1, new_fd, i;

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

        for (i = 0; i < nfds; i++) {
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
                    printf("Received: %s\n", buffer);
                    char httpResponse[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello world!";
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
