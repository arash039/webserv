# Webserv

Webserv is a C++ web server implementation that can handle multiple server configurations and connections. It uses the poll() system call to monitor and handle incoming connections and requests.
Features

- Supports multiple server configurations and connections
- Handles HTTP requests and responses
- Supports chunked transfer encoding
- Implements a custom logger for debugging and logging purposes
- Handles request timeouts and connection closures

## Usage

To use Webserv, you need to provide a configuration file as a command-line argument. The configuration file should be in the correct format, as expected by the configParser class. Few examples are included in ConfogFiles directory.

```bash
./webserv <config_file>
```

The server will start and listen for incoming connections on the specified ports. You can monitor the server's activity and any errors or warnings through the custom logger.
Code Structure

The main entry point of the application is the main() function, which sets up the server configurations, connections, and the event loop. The key components of the code are:

- configParser: Responsible for parsing the configuration file and providing the server configurations.
- Connection: Represents a server connection, handling socket setup and poll() setup.
- HTTPRequest: Handles the parsing and processing of incoming HTTP requests.
- Response: Generates and sends the appropriate HTTP responses.
- logger: Provides a custom logging mechanism for debugging and informational purposes.

## File Descriptor Management
In the code, the main file descriptor management is done in the following parts:

### Initializing File Descriptors:
- The code creates a Connection object for each server configuration, which sets up the socket and the poll() file descriptor array (fds) for that connection.
- The fcntl() system call is used to set the listen socket to non-blocking mode (O_NONBLOCK).
- The file descriptors for the listen sockets are added to the active_fds set, which keeps track of all active file descriptors.
### Accepting New Connections:
- When the poll() call indicates that the listen socket is ready for reading, the code calls accept() to accept the new connection.
- The new file descriptor is set to non-blocking mode and added to the active_fds set, as well as the last_data_time map to keep track of the last time data was received on the connection.
- The new file descriptor is also added to the fds array of the corresponding Connection object.
### Handling Incoming Data:
- When the poll() call indicates that a file descriptor is ready for reading, the code reads the data from the socket into a buffer.
- The last_data_time map is updated with the current time to keep track of the last time data was received on the connection.
- The received data is then passed to the HTTPRequest object for parsing and processing.
### Handling Timeouts:
- The handleTimeout() function is called for each file descriptor to check if the connection has timed out (i.e., no data received within the configured timeout period).
- If a connection has timed out, the code sends a "408 Request Timeout" response, closes the connection, and removes the file descriptor from the active_fds set and the last_data_time map.
### Closing Connections:
- When the poll() call indicates that a file descriptor has been closed (i.e., recv() returns 0), the code closes the connection, removes the file descriptor from the active_fds set and the last_data_time map, and updates the fds array accordingly.

## poll() Usage
The code uses the poll() system call in the main event loop to monitor the file descriptors for incoming data and new connections. The key points about the poll() usage are:
- Polling Connections: The poll() call is made for each Connection object, with the fds array and the number of file descriptors (nfds) passed as arguments.
- Timeout: The poll() call uses a timeout value of 10 milliseconds, which means the call will block for up to 10 milliseconds waiting for a file descriptor to become ready.
- Handling Events: The code iterates over the fds array and checks the revents field to determine the type of event that occurred (e.g., read readiness, write readiness, error).
- Error Handling: The code checks the return value of poll() and handles any errors that may occur, such as by logging the error and continuing to the next iteration of the loop.

Overall, the code uses the poll() system call effectively to monitor and manage the file descriptors associated with the server connections, handling new connections, incoming data, timeouts, and connection closures.
