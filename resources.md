https://www.youtube.com/watch?v=bdIiTxtMaKA&list=PL9IEJIKnBJjH_zM5LnovnoaKlXML5qh17&index=1&t=1s
https://nginx.org/en/docs/http/request_processing.html
https://man7.org/linux/man-pages/man3/sockaddr.3type.html


# functions
- `strerror`: This function returns a pointer to a text string that describes the error code passed in the argument errno
- `gai_strerror`: This function returns a pointer to a text string that describes the error value returned by a failure return from either the `getaddrinfo` or `getnameinfo`.
- `errno`: `errno` is a preprocessor macro used for error indication. [It expands to a static (until C++11) thread-local (since C++11) modifiable lvalue of type int7]
- `socketpair`: This function creates a pair of connected sockets
- `htons`: This function converts a `u_short` from host to TCP/IP network byte order (which is big-endian).
- `htonl`: Similar to `htons`, but for long integers.
- `ntohs`: This function converts a `u_short` from TCP/IP network byte order to host byte order.
- `ntohl`: Similar to `ntohs`, but for long integers.
- `select`: This function allows a program to monitor multiple file descriptors, waiting until one or more of the file descriptors become “ready” for some class of I/O operation.
- `poll`: Similar to `select`, but with a more detailed event reporting mechanism.
- `epoll_create`, `epoll_ctl`, `epoll_wait`: These functions provide a scalable I/O event notification mechanism. `epoll_create` creates an epoll instance, `epoll_ctl` adds, modifies, or removes entries in the instance, and `epoll_wait` waits for events on the epoll instance.
- `kqueue`, `kevent`: These functions provide a scalable I/O event notification mechanism on BSD systems, similar to epoll on Linux.
- `socket`: This function creates an endpoint for communication and returns a descriptor.
- `accept`: This function is used to accept a new connection on a socket.
- `listen`: This function listens for connections on a socket.
- `send`, `recv`: These functions are used to send and receive messages from a socket, respectively.
- `chdir`: This function changes the current working directory of the calling process.
- `bind`: This function assigns a local protocol address to a socket.
- `connect`: This function initiates a connection on a socket.
- `getaddrinfo`, `freeaddrinfo`: These functions are used to resolve network names and addresses.
- `setsockopt`: This function sets the options associated with a socket.
- `getsockname`: This function retrieves the locally-bound name of the specified socket.
- `getprotobyname`: This function returns protocol information corresponding to a protocol name.
- `access`: This function checks the caller’s permissions for a file.
- `stat`: This function returns information about a file.
- `opendir`, `readdir`, `closedir`: These functions are used to open, read from, and close a directory stream, respectively.