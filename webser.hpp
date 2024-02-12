#ifndef WEBSER_HPP
#define WEBSER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/wait.h>


#define MAX_CLIENTS 100

struct LocationConfig {
    std::string path;
    std::string root;
    std::vector<std::string> methods;
    std::string redirection;
    bool allowDirectoryListing;
    std::string indexFile;
    std::string cgiExtension;
};

struct ServerConfig {
    int port;
	int clientMaxBodySize;
    std::string host;
    std::vector<std::string> serverNames;
    std::string errorPage404;
    std::string errorPage50x;
    std::vector<LocationConfig> locations;
};

class WebServer {
public:
	std::map<std::string, std::string> requestMap;
	std::map<std::string, std::string> configMap;
	void setConfigMap(std::map<std::string, std::string> m) {
		configMap = m;
	};
	std::map<std::string, std::string> getConfigMap() {
		return configMap;
	};
	void setMap(std::map<std::string, std::string> m) {
		requestMap = m;
	};
	std::map<std::string, std::string> getMap() {
		return requestMap;
	};
	void printMap() {
		for (auto const& x : requestMap) {
			std::cout << "#" << x.first << " " << x.second << std::endl;
		}
	};
};

bool parseNginxConfig(const std::string& filename, std::vector<ServerConfig>& servers);
bool separateKeyValue(const std::string& line, std::string& key, std::string& value);
void printParsedResults(const std::vector<ServerConfig>& servers);



#endif