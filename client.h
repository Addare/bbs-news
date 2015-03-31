#include <iostream>
#include <string>
#include <stdexcept>
#include <map>

#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"
#include "messagehandler.h"

struct Client {
	Messagehandler msgHnd;
	std::string mode = "news";
	std::map<std::string, void (Client::*)()> cmd;
	
	Client(Connection& c);
	
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
	void get_art();
};


