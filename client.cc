#include "client.h"

void dbgCode(int a, int b) {
	if (a != b)
		std::cerr << "Incorrect code from server, expected: " << a << ", but got: " << b << std::endl;
}

Client::Client(Connection& c) : msgHnd(c) {
	cmd["quit"] = &Client::cmd_quit;
	cmd["help"] = &Client::cmd_help;

	cmd["list_news"] = &Client::list_ng;
	cmd["create_news"] = &Client::create_ng;
	cmd["delete_news"] = &Client::delete_ng;
	
	cmd["list_article"] = &Client::list_art;
	cmd["create_article"] = &Client::create_art;
	cmd["delete_article"] = &Client::delete_art;
	cmd["get_article"] = &Client::get_art;
}
	
void Client::cmd_quit() { exit(0); }

void Client::cmd_help() {
	std::cout << "\tquit             \t\t Quit the program" << std::endl;
	std::cout << "\thelp             \t\t Print this help text" << std::endl;
	std::cout << std::endl;
	std::cout << "\tnews [command]   \t\t Edit newsgroups" << std::endl;
	std::cout << "\t.... list        \t\t List all newsgroups stored on the server" << std::endl;
	std::cout << "\t.... create      \t\t Create a new newsgroup" << std::endl;
	std::cout << "\t.... delete      \t\t Delete a specified newsgroup" << std::endl;
	std::cout << std::endl;
	std::cout << "\tarticle [command]\t\t Edit articles" << std::endl;
	std::cout << "\t.... list        \t\t List all articles in the specified newsgroup stored on the server" << std::endl;
	std::cout << "\t.... get         \t\t Get single article in specified newsgroup" << std::endl;
	std::cout << "\t.... create      \t\t Create a new article in the specified newsgroup" << std::endl;
	std::cout << "\t.... delete      \t\t Delete an article in the specified newsgroup" << std::endl;
}

void Client::list_ng() {
	msgHnd.sendCode(Protocol::COM_LIST_NG);
	msgHnd.sendCode(Protocol::COM_END);

	int code = msgHnd.recCode();
	dbgCode(Protocol::ANS_LIST_NG, code);

	int n = msgHnd.recInt();
	std::cout << "#" << n << " newsgroups received" << std::endl;
	int i = 0;
	while (i < n) {
		int id = msgHnd.recInt();
		std::string title = msgHnd.recString();
		std::cout << "[" << id << "]: " << title << std::endl;

		i++;
	}
	
	code = msgHnd.recCode();
	dbgCode(Protocol::ANS_END, code);
}

void Client::create_ng() {
	std::string title;
	std::cout << "newgroup-title> "; std::getline(std::cin, title);

	msgHnd.sendCode(Protocol::COM_CREATE_NG);
	msgHnd.sendString(title);
	msgHnd.sendCode(Protocol::COM_END);

	int code = msgHnd.recCode();
	dbgCode(Protocol::ANS_CREATE_NG, code);

	code = msgHnd.recCode();
	if (code == Protocol::ANS_NAK) {
		code = msgHnd.recCode(); // error-code
		dbgCode(Protocol::ERR_NG_ALREADY_EXISTS, code);
		std::cerr << "ERROR: unable to create newsgroup, newwsgroup already exists" << std::endl;
	} else {
		dbgCode(Protocol::ANS_ACK, code);
	}	

	code = msgHnd.recCode();
	dbgCode(Protocol::ANS_END, code);
}

void Client::delete_ng() {
	int id;
	std::cout << "article-id> "; std::cin >> id; 
	std::cin.ignore();

	msgHnd.sendCode(Protocol::COM_DELETE_NG);
	msgHnd.sendInt(id);
	msgHnd.sendCode(Protocol::COM_END);

	int code = msgHnd.recCode();
	dbgCode(Protocol::ANS_DELETE_NG, code);

	code = msgHnd.recCode();
	if (code == Protocol::ANS_NAK) {
		code = msgHnd.recCode(); // error-code
		dbgCode(Protocol::ERR_NG_DOES_NOT_EXIST, code);
		std::cerr << "ERROR: Unable to delete newsgroup, newsgroup does not exist" << std::endl;
	} else {
		dbgCode(Protocol::ANS_ACK, code);
	}

	code = msgHnd.recCode();
	dbgCode(Protocol::ANS_END, code);
}

void Client::list_art() {
	int id;
	std::cout << "newsgroup-id> "; std::cin >> id;
	std::cin.ignore();

	msgHnd.sendCode(Protocol::COM_LIST_ART);
	msgHnd.sendInt(id);
	msgHnd.sendCode(Protocol::COM_END);

	int code = msgHnd.recCode();
	dbgCode(Protocol::ANS_LIST_ART, code);

	code = msgHnd.recCode();
	if (code == Protocol::ANS_ACK) {
		int n = msgHnd.recInt();
		std::cout << "#" << n << " articles received" << std::endl;

		int i = 0;
		while (i < n) {
			int id = msgHnd.recInt();
			std::string title = msgHnd.recString();
			std::cout << "[" << id << "]: " << title << std::endl;

			i++;
		}
	} else {
		dbgCode(Protocol::ANS_NAK, code);
		code = msgHnd.recCode(); // error-code
		dbgCode(Protocol::ERR_NG_DOES_NOT_EXIST, code);
		std::cerr << "ERROR! Unable to list article in given newsgroup, newsgroup does not exist!" << std::endl;
	}

	code = msgHnd.recCode();
	dbgCode(Protocol::ANS_END, code);
}

void Client::create_art() {
	int id;
	std::string title, author, text;

	std::cout << "newsgroup-id> "; std::cin >> id;
	std::cin.ignore();

	std::cout << "article-title> "; std::getline(std::cin, title);
	std::cout << "article-author> "; std::getline(std::cin, author);
	std::cout << "article-text> "; std::getline(std::cin, text);

	msgHnd.sendCode(Protocol::COM_CREATE_ART);
	msgHnd.sendInt(id);
	msgHnd.sendString(title);
	msgHnd.sendString(author);
	msgHnd.sendString(text);
	msgHnd.sendCode(Protocol::COM_END);

	int code = msgHnd.recCode();
	dbgCode(Protocol::ANS_CREATE_ART, code);
	
	code = msgHnd.recCode();
	if (code == Protocol::ANS_NAK) {
		code = msgHnd.recCode(); // error-code
		dbgCode(Protocol::ERR_NG_DOES_NOT_EXIST, code);
		std::cerr << "ERROR! Unable to create article in given newsgroup, newsgroup does not exist!" << std::endl;
	} else {
		dbgCode(Protocol::ANS_ACK, code);
	}

	code = msgHnd.recCode();
	dbgCode(Protocol::ANS_END, code);
}

void Client::delete_art() {
	int gid, aid;
	std::cout << "newsgroup-id> "; std::cin >> gid;
	std::cout << "article-id> "; std::cin >> aid;
	std::cin.ignore();

	msgHnd.sendCode(Protocol::COM_DELETE_ART);
	msgHnd.sendInt(gid);
	msgHnd.sendInt(aid);
	msgHnd.sendCode(Protocol::COM_END);

	int code = msgHnd.recCode();
	dbgCode(Protocol::ANS_DELETE_ART, code);

	code = msgHnd.recCode();
	if (code == Protocol::ANS_NAK) {
		code = msgHnd.recCode(); // error-code
		if (code == Protocol::ERR_NG_DOES_NOT_EXIST) {
			std::cerr << "ERROR! Unable to delete article in given newsgroup, newsgroup does not exist!" << std::endl;
		} else {
			dbgCode(Protocol::ERR_ART_DOES_NOT_EXIST, code);
			std::cerr << "ERROR! Unable to delete article, article does not exist!" << std::endl;
		}
	} else {
		dbgCode(Protocol::ANS_ACK, code);
	}
	
	code = msgHnd.recCode();
	dbgCode(Protocol::ANS_END, code);
}

void Client::get_art() {
	int gid, aid;
	std::cout << "newsgroup-id> "; std::cin >> gid;
	std::cout << "article-id> "; std::cin >> aid;
	std::cin.ignore();

	msgHnd.sendCode(Protocol::COM_GET_ART);
	msgHnd.sendInt(gid);
	msgHnd.sendInt(aid);
	msgHnd.sendCode(Protocol::COM_END);

	int code = msgHnd.recCode();
	dbgCode(Protocol::ANS_GET_ART, code);

	code = msgHnd.recCode();
	if (code == Protocol::ANS_ACK) {
		std::string title = msgHnd.recString();
		std::string author = msgHnd.recString();
		std::string text = msgHnd.recString();

		std::cout << "-- " << title << " --- Written by: " << author << std::endl;

		unsigned int line_length = title.length() + author.length() + 20;
		for (unsigned int i = 0; i < line_length; i++)
			std::cout << "-";
		std::cout << std::endl;

		std::cout << text << std::endl;

	} else {
		code = msgHnd.recCode(); // error-code
		if (code == Protocol::ERR_NG_DOES_NOT_EXIST) 
			std::cerr << "ERROR! Unable to get article in given newsgroup, newsgroup does not exist!" << std::endl;
		else { 
			dbgCode(Protocol::ERR_ART_DOES_NOT_EXIST, code);
			std::cerr << "ERROR! Unable to get given article, article does not exist!" << std::endl;
		}
	}

	code = msgHnd.recCode();
	dbgCode(Protocol::ANS_END, code);
}

int main(int argc, char* argv[]) {

	if (argc != 3) {
		std::cerr << "Usage: client host port" << std::endl;
		exit(1);
	}

	int port = 4242;
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

	Client client(conn);


	std::cout << "bbs-news> ";

	std::string tmp;
	while (std::getline(std::cin, tmp)) {
		std::stringstream input(tmp);
		std::string mode, command;
		
		input >> mode;
		if (input.eof()) {
			client.cmd_help();
			std::cout << "bbs-news> ";
			continue;
		}

		input >> command;

		if (mode != "news" && mode != "article") {
			auto cmd = client.cmd.find(command);
			if (cmd != client.cmd.end()) 
				(client.*cmd->second)();
			else 
				client.cmd_help();
			
			std::cout << "bbs-news> ";
			continue;
		}

		auto cmd = client.cmd.find(command + "_" + mode);
		if (cmd == client.cmd.end())
			 client.cmd_help();
		else 
			(client.*cmd->second)();
		
		std::cout << "bbs-news> ";
	}
}

