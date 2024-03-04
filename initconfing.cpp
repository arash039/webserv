#include "webser.hpp"

void initconfig(ServerConfig &config) {
	config.port = 8080;
	config.clientMaxBodySize = 1000000;
	config.host = "localhost";
	config.serverNames.push_back("localhost");
	config.errorPage404 = "error404.html";
	config.errorPage50x = "error50x.html";
	LocationConfig location;
	location.path = "/";
	location.root = "/var/www/html";
	location.methods.push_back("GET");
	location.methods.push_back("POST");
	location.redirection = "http://localhost:8080";
	location.allowDirectoryListing = true;
	location.indexFile = "index.html";
	location.cgiExtension = ".php";
	location.tryFiles = "index.html";
	config.locations.push_back(location);
	LocationConfig location2;
	location2.path = "/kapouet";
	location2.root = "/tmp/www";
	location2.methods.push_back("GET");
	location2.methods.push_back("POST");
	location2.redirection = "http://localhost:8080";
	location2.allowDirectoryListing = true;
	location2.indexFile = "index.html";
	location2.cgiExtension = ".php";
	location2.tryFiles = "index.html";
	config.locations.push_back(location2);
	LocationConfig location3;
	location3.path = "/var/www/html/images";
	location3.root = "/var/www/html/images";
	location3.methods.push_back("GET");
	location3.methods.push_back("POST");
	location3.redirection = "http://localhost:8080";
	location3.allowDirectoryListing = true;
	location3.indexFile = "index.html";
	location3.cgiExtension = ".php";
	location3.tryFiles = "index.html";
	config.locations.push_back(location3);
}