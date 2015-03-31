#include <iostream>
#include <string>
#include <stdexcept>
#include <map>

#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"
//#include "messagehandler.h"

void mode_ng();
void mode_art();
void cmd_quit();
void cmd_help();

void list_ng();
void create_ng();
void delete_ng();

void list_art();
void create_art();
void delete_art();

struct Client {
	std::string mode = "news";
	std::map<std::string, void (*)()> cmd;
	
	Client() {
		cmd["news"] = &mode_ng;
		cmd["article"] = &mode_art;
		cmd["quit"] = &cmd_quit;
		cmd["help"] = &cmd_help;

		cmd["list_ng"] = &list_ng;
		cmd["create_ng"] = &create_ng;
		cmd["delete_ng"] = &delete_ng;
		
		cmd["list_art"] = &list_art;
		cmd["create_art"] = &create_art;
		cmd["delete_art"] = &delete_art;
	};
};


int main(int argc, char* argv[]) {
	Client client;

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

	/**Connection conn(argv[1], port);
	if (!conn.isConnected()) {
		std::cerr << "Connection attempt failed" << std::endl;
		exit(1);
	}**/

	std::cout << client.mode << "> ";

	std::string command;
	while (std::cin >> command) {
		auto cmd = client.cmd.find(command);
		if (cmd == client.cmd.end())
			client.cmd["help"]();
		else 
			cmd->second();
		
		std::cout << client.mode << "> ";
	}
}

void mode_ng() {
	std::cerr << "stub! " << __FILE__ << ":" << __func__ << ":" << __LINE__ << std::endl;
}

void mode_art() {
	std::cerr << "stub! " << __FILE__ << ":" << __func__ << ":" << __LINE__ << std::endl;
}

void cmd_quit() {
	std::cerr << "stub! " << __FILE__ << ":" << __func__ << ":" << __LINE__ << std::endl;
}

void cmd_help() {
	std::cerr << "stub! " << __FILE__ << ":" << __func__ << ":" << __LINE__ << std::endl;
}

void list_ng() {
	std::cerr << "stub! " << __FILE__ << ":" << __func__ << ":" << __LINE__ << std::endl;
}

void create_ng() {
	std::cerr << "stub! " << __FILE__ << ":" << __func__ << ":" << __LINE__ << std::endl;
}

void delete_ng() {
	std::cerr << "stub! " << __FILE__ << ":" << __func__ << ":" << __LINE__ << std::endl;
}

void list_art() {
	std::cerr << "stub! " << __FILE__ << ":" << __func__ << ":" << __LINE__ << std::endl;
}

void create_art() {
	std::cerr << "stub! " << __FILE__ << ":" << __func__ << ":" << __LINE__ << std::endl;
}

void delete_art() {
	std::cerr << "stub! " << __FILE__ << ":" << __func__ << ":" << __LINE__ << std::endl;
}
