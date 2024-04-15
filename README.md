Webserv
Webserv is a C++ web server implementation that can handle multiple server configurations and connections. It uses the poll() system call to monitor and handle incoming connections and requests.
Features

    Supports multiple server configurations and connections
    Handles HTTP requests and responses
    Supports chunked transfer encoding
    Implements a custom logger for debugging and logging purposes
    Handles request timeouts and connection closures

Usage
To use Webserv, you need to provide a configuration file as a command-line argument. The configuration file should be in the correct format, as expected by the configParser class.

./webserv <config_file>

The server will start and listen for incoming connections on the specified ports. You can monitor the server's activity and any errors or warnings through the custom logger.
Code Structure
The main entry point of the application is the main() function, which sets up the server configurations, connections, and the event loop. The key components of the code are:

    configParser: Responsible for parsing the configuration file and providing the server configurations.
    Connection: Represents a server connection, handling socket setup and poll() setup.
    HTTPRequest: Handles the parsing and processing of incoming HTTP requests.
    Response: Generates and sends the appropriate HTTP responses.
    logger: Provides a custom logging mechanism for debugging and informational purposes.

The code also includes various utility functions and error handling mechanisms to ensure the server's stability and reliability.
