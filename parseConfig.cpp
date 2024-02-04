#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

struct LocationConfig {
    string path;
    string root;
    vector<string> methods;
    string redirection;
    bool allowDirectoryListing;
    string indexFile;
    string cgiExtension;
};

struct ServerConfig {
    int port;
	int clientMaxBodySize;
    string host;
    vector<string> serverNames;
    string errorPage404;
    string errorPage50x;
    vector<LocationConfig> locations;
};

bool separateKeyValue(const std::string& line, std::string& key, std::string& value) {
    // Trim leading and trailing whitespaces
    size_t first_non_space = line.find_first_not_of(" \t");
    size_t last_non_space = line.find_last_not_of(" \t");
    if (first_non_space == std::string::npos || last_non_space == std::string::npos) {
        // Handle empty or all-whitespace lines
        return false;
    }
    std::string trimmed_line = line.substr(first_non_space, last_non_space - first_non_space + 1);

    // Find the separator (first whitespace after the key)
    size_t separator_pos = trimmed_line.find_first_of(" \t");
    if (separator_pos == std::string::npos) {
        // Handle lines without a separator
        return false;
    }

    // Extract key and value
    key = trimmed_line.substr(0, separator_pos);
    value = trimmed_line.substr(separator_pos + 1);  // Skip the whitespace

    // Trim leading and trailing whitespaces from value
    value = value.substr(value.find_first_not_of(" \t"), value.find_last_not_of(" \t") + 1);
	value = value.substr(0, value.find(";"));
    return true;
}

bool parseNginxConfig(const string& filename, vector<ServerConfig>& servers) {
	ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return false;
    }

    string line;
	ServerConfig currentServer;
	currentServer.port = 0;
	LocationConfig currentLocation;
	currentLocation.path = "";
	
    while (getline(file, line)) {
		std::string key, value;
        if (line.empty() || line[0] == '#') {
            continue;  // Skip empty lines and comments
        }
		if (line.find("server") != string::npos && line.find("server_name") == string::npos) {
			// Start of a new server block
			if (currentServer.port != 0) {
				servers.push_back(currentServer);
				currentServer = ServerConfig();
			}
		} else if (line.find("location") != string::npos) {
			// Start of a new location block
			if (!currentLocation.path.empty()) {
				currentServer.locations.push_back(currentLocation);
				currentLocation = LocationConfig();
			}
			separateKeyValue(line, key, value);
			currentLocation.path = value.substr(0, value.size() - 2);
		} /* else if (line.find("}") != string::npos) {
			// End of a server or location block
			if (currentLocation.path.empty()) {
				currentServer.locations.push_back(currentLocation);
			} else {
				servers.push_back(currentServer);
			}
		}  */else if (line.find("listen") != string::npos) {
			// Parse the port number
			separateKeyValue(line, key, value);
			currentServer.port = stoi(value);
		} else if (line.find("server_name") != string::npos) {
			// Parse the server names
			separateKeyValue(line, key, value);
			currentServer.serverNames.push_back(value);
		} else if (line.find("root") != string::npos) {
			// Parse the root directory
			separateKeyValue(line, key, value);
			currentLocation.root = value;
		} else if (line.find("allow") != string::npos) {
			// Parse the allowed methods
			separateKeyValue(line, key, value);
			currentLocation.methods.push_back(value);
		} else if (line.find("return") != string::npos) {
			// Parse the redirection URL
			separateKeyValue(line, key, value);
			currentLocation.redirection = value;
		} else if (line.find("autoindex") != string::npos) {
			// Parse the directory listing setting
			separateKeyValue(line, key, value);
			currentLocation.allowDirectoryListing = (value == "on");
		} else if (line.find("index") != string::npos) {
			// Parse the index file
			separateKeyValue(line, key, value);
			currentLocation.indexFile = value;
		} else if (line.find("fastcgi_param") != string::npos) {
			// Parse the CGI extension
			separateKeyValue(line, key, value);
			currentLocation.cgiExtension = value;
		} else if (line.find("server_name") != string::npos) {
			// Parse the server names
			separateKeyValue(line, key, value);
			currentServer.serverNames.push_back(value);
		} else if (line.find("client_max_body_size") != string::npos) {
			key = "client_max_body_size";
			separateKeyValue(line, key, value);
			currentServer.clientMaxBodySize = stoi(value);
		} 
	}
	
	file.close();
	if (!currentLocation.path.empty()) {
		currentServer.locations.push_back(currentLocation);
	}
	if (currentServer.port != 0) {
		servers.push_back(currentServer);
	}
    return true;
}

void printParsedResults(const std::vector<ServerConfig>& servers) {
    cout << "Parsed Nginx configuration:" << endl;

    for (const ServerConfig& server : servers) {
        cout << "Server:" << endl;
        cout << "  Port: " << server.port << endl;
        cout << "  Host: " << server.host << endl;
        cout << "  Server names: ";
        for (const string& name : server.serverNames) {
            cout << name << " ";
        }
        cout << endl;
        cout << "  Error pages:" << endl;
        cout << "    404: " << server.errorPage404 << endl;
        cout << "    50x: " << server.errorPage50x << endl;
        cout << "  Locations:" << endl;
		int i = 1;
        for (const LocationConfig& location : server.locations) {
			cout << "  Location " << i++ << ":" << endl;
            cout << "    Path: " << location.path << endl;
            cout << "    Root: " << location.root << endl;
            cout << "    Methods: ";
            for (const string& method : location.methods) {
                cout << method << " ";
            }
            cout << endl;
            cout << "    Redirection: " << location.redirection << endl;
            cout << "    Directory listing: " << (location.allowDirectoryListing ? "enabled" : "disabled") << endl;
            cout << "    Index file: " << location.indexFile << endl;
            cout << "    CGI extension: " << location.cgiExtension << endl;
        }
        cout << endl;
    }
}

/* int main() {
    vector<ServerConfig> servers;
    if (parseNginxConfig("config1", servers)) {
        // Process parsed configuration data
    } else {
        cerr << "Error parsing configuration file." << endl;
    }
	printParsedResults(servers);
    return 0;
} */
