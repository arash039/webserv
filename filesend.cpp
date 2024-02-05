#include "webser.hpp"

void sendResponse(int clientSocket, std::string httpResponse, std::string response_data) {
    // Send the HTTP headers
    send(clientSocket, httpResponse.c_str(), httpResponse.size(), 0);
    // Send the file content
    send(clientSocket, response_data.c_str(), response_data.size(), 0);
}

void directoryListing(int clientSocket, std::string path) {
	std::string response_data = "<!DOCTYPE html><html><body>";
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(path.c_str())) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			response_data += "<a href=\"" + std::string(ent->d_name) + "\">" + std::string(ent->d_name) + "</a><br>";
		}
		closedir(dir);
	} else {
		perror("opendir");
	}
	response_data += "</body></html>";
	std::string httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(response_data.size()) + "\r\n\r\n";
	sendResponse(clientSocket, httpResponse, response_data);
}

void parseRequest(std::string request, class WebServer &w) {
	std::map<std::string, std::string> requestMap;
	std::string delimiter = "\r\n";
	size_t pos = 0;
	std::string token;
	while ((pos = request.find(delimiter)) != std::string::npos) {
		token = request.substr(0, pos);
		request.erase(0, pos + delimiter.length());
		if (token.find("GET") != std::string::npos) {
			std::string token2;
			token2 = token.substr(4, token.find("HTTP/1.1") - 5);
			requestMap["GET"] = token2;
		}
		else {
		std::string delimiter2 = ": ";
		size_t pos2 = 0;
		std::string token2;
		pos2 = token.find(delimiter2);
		token2 = token.substr(0, pos2);
		token.erase(0, pos2 + delimiter2.length());
		requestMap[token2] = token;
		}
	}
	w.setMap(requestMap);
}

void generateResponse(class WebServer &w) {
	std::map<std::string, std::string> requestMap = w.getMap();
	std::map<std::string, std::string> configMap = w.getConfigMap();
	std::string response = "HTTP/1.1 200 OK\r\n";
	response += "Server: WebServer/1.0\r\n";
	response += "Content-Type: text/html\r\n";
	response += "Content-Length: 0\r\n";
	response += "Connection: close\r\n";
	response += "\r\n";
}

std::string findFile(std::string path, std::string root, std::vector<ServerConfig> &servers) {
	std::string fullPath = "";
	if (path == "/") {
		fullPath = "." + root + "/index.html";
	}
	else {
		for(auto &server : servers) {
			for(auto &location : server.locations) {
				if (path == location.path) {
					fullPath = "." + location.root + path + ".html";
				}
			}
		}
	}
	std::cout << fullPath << std::endl;
	std::ifstream html_data(fullPath);
	std::string response_data;
	if (!html_data) {
		std::cerr << "Could not open file\n";
		return "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 0\r\n\r\n";
		//return "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 6\r\n\r\n Hello";
	}
	std::stringstream buff;
	buff << html_data.rdbuf();
	response_data = buff.str();
	std::string httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(response_data.size()) + "\r\n\r\n" + response_data;
	return (httpResponse);
}

int main() {
	std::vector<ServerConfig> servers;
	parseNginxConfig("config1", servers);
	int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;
	class WebServer w;
	//tells the operating system to not reserve the socket after the 
	//program is terminated, allowing you to immediately rerun your 
	//program without waiting for the operating system to release the socket. 
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("setsockopt");
        return 1;
    }
    sockaddr_in server_addr;
    struct pollfd fds[MAX_CLIENTS];
    int nfds = 1, new_fd;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
	//This is an IP address that is used when we don't want to bind a 
	//socket to any specific IP. Basically, while implementing 
	//communication, we need to bind our socket to an IP address. 
	//When we don't know the IP address of our machine, we can use the 
	//special IP address INADDR_ANY.
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(listen_fd, 10);

    fds[0].fd = listen_fd;
    fds[0].events = POLLIN;
	int connectionHandled = 0;
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
                    std::cout << "Received: " << std::endl << buffer << std::endl;
					parseRequest(buffer, w);
					// Open the HTML file
                    /* std::ifstream html_data("index.html");

                    if (!html_data) {
                        std::cerr << "Could not open file\n";
                        return 1;
                    } */

                    // Read the file into memory
                    /* std::stringstream buff;
                    buff << html_data.rdbuf();
                    std::string response_data = buff.str(); */

                    // Prepare the HTTP response
                    //std::string httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(response_data.size()) + "\r\n\r\n" + response_data;
					std::string httpResponse = findFile(w.requestMap["GET"], servers[0].locations[0].root, servers);
					write(fds[i].fd, httpResponse.c_str(), httpResponse.size());
					//directoryListing(fds[i].fd, servers[0].locations[0].root);
				} else {
                    close(fds[i].fd);
                    fds[i] = fds[nfds - 1];
                    nfds--;
                }
	            connectionHandled++;
            }
        }
		/* if (connectionHandled == 1)
    		break; */
    }
	//w.printMap();
    return 0;
}
