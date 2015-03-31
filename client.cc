#include <iostream>
#include <string>
#include <stdexcept>

#include "connection.h"
#include "connectionclosedexception.h"
#include "messagehandler.h"

int main(int argc, char* argv[]) {
	if (argc != 3) {
		std::cerr << "Usage: client host port" << std::endl;
		exit(1);
	}

	int port = -1;
	try {
		port = std::stoi(argv[2]);
	} catch (std::exception& e) {
	 	std::cerr << "Wrong port number. " << e.what() << std::endl;
		exit(1);
	}	

	Connection conn(argv[1], port);
	if (!conn.isConnected()) {
		std::cerr << "Connection attempt failed" << std::endl;
		exit(1);
	}

	std::string command;
	while (std::cin >> command) {
		std::cout << command << std::endl << "> ";
	}
}
