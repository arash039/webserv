#include "webser.hpp"

void errorPageParser(const std::string& line, std::string& errorPage404, std::string& errorPage50x) {
	size_t separator_pos = line.find_first_of("/");
	size_t end_pos = line.find_last_of(";");
	if (separator_pos == std::string::npos) {
		return;
	}
	std::string key = line.substr(0, separator_pos);
	std::string value = line.substr(separator_pos + 1, end_pos - separator_pos);
	if (key.find("404") != std::string::npos) {
		errorPage404 = value;
	} else if (key.find("50") != std::string::npos) {
		errorPage50x = value;
	}
}

int main() {
	ServerConfig serverConfig;
	std::ifstream file("config1");
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << "config1" << std::endl;
        return false;
    }
	std::string line;
	while (std::getline(file, line)) {
		std::string key, value;
		if (line.find("error_page") != std::string::npos) {
			errorPageParser(line, serverConfig.errorPage404, serverConfig.errorPage50x);
		}
	}
	std::cout << "errorPage404: " << serverConfig.errorPage404 << std::endl;
	std::cout << "errorPage50x: " << serverConfig.errorPage50x << std::endl;
	file.close(); 
	return 0;
}


