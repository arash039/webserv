#include <sys/socket.h>
#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

// ... other includes

int main() {
  // Create and set non-blocking flag on listening socket
  int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_fd == -1) {
    perror("socket");
    return 1;
  }
  if (fcntl(listen_fd, F_SETFL, fcntl(listen_fd, F_GETFL, 0) | O_NONBLOCK) == -1) {
    perror("fcntl");
    return 1;
  }

  // Bind and listen on the socket
  // ... (remaining code for binding and listening)

  int nfds = 1; // Initialize with listen_fd
  fd_set readfds;

  while (1) {
    FD_ZERO(&readfds);
    FD_SET(listen_fd, &readfds);

    // Select with a timeout to avoid busy-waiting
    int n = select(nfds, &readfds, nullptr, nullptr, nullptr);
    if (n == -1) {
      perror("select");
      return 1;
    } else if (n == 0) {
      // Handle timeout if needed
      continue;
    }

    // Check for events on each descriptor
    for (int i = 0; i < nfds; ++i) {
      if (FD_ISSET(i, &readfds)) {
        if (i == 0) { // New connection on listen_fd
          int new_fd = accept4(listen_fd, nullptr, nullptr, SOCK_NONBLOCK);
          if (new_fd == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
              // No new connections available, continue
              continue;
            } else {
              perror("accept");
              return 1;
            }
          }
          FD_SET(new_fd, &readfds);
          nfds++;
        } else { // Data available on existing connection
          char buffer[1024];
          int len = read(i, buffer, sizeof(buffer) - 1);
          if (len == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
              // No data available yet, continue
              continue;
            } else {
              perror("read");
              close(i);
              FD_CLR(i, &readfds);
              nfds--;
              continue;
            }
          } else if (len == 0) { // Client closed connection
            close(i);
            FD_CLR(i, &readfds);
            nfds--;
            continue;
          }
          buffer[len] = '\0';
          std::cout << "Received: " << std::endl << buffer << std::endl;
          parseRequest(buffer, w);
        }
      }
    }
  }

  // Close sockets and resources
  return 0;
}
