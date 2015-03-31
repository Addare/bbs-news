#include "client.h"

void dbgCode(int a, int b) {
	if (a != b)
		std::cerr << "Incorrect code from server, expected: " << a << ", but got: " << b << std::endl;
}

Client::Client(Connection& c) : msgHnd(c) {
	cmd["news"] = &Client::mode_ng;
	cmd["article"] = &Client::mode_art;
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
	
void Client::mode_ng() { mode = "news"; }
void Client::mode_art() { mode = "article"; }
void Client::cmd_quit() { exit(0); }

void Client::cmd_help() {
	std::cerr << "stub! " << __FILE__ << ":" << __func__ << ":" << __LINE__ << std::endl;
}

void Client::list_ng() {
	msgHnd.sendCode(Protocol::COM_LIST_NG);
	msgHnd.sendCode(Protocol::COM_END);

	int code = msgHnd.recCode();
	dbgCode(Protocol::ANS_LIST_NG, code);

	int n = msgHnd.recInt();
	int i = 0;
	while (i < n) {
		int id = msgHnd.recInt();
		std::string title = msgHnd.recString();
		std::cout << "[" << id << "]: " << title;

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
	} else {
		dbgCode(Protocol::ANS_ACK, code);
	}

	code = msgHnd.recCode();
	dbgCode(Protocol::ANS_END, code);
}

void Client::list_art() {
	int id;
	std::cout << "article-id> "; std::cin >> id;
	std::cin.ignore();

	msgHnd.sendCode(Protocol::COM_LIST_ART);
	msgHnd.sendInt(id);
	msgHnd.sendCode(Protocol::COM_END);

	int code = msgHnd.recCode();
	dbgCode(Protocol::ANS_LIST_ART, code);

	code = msgHnd.recCode();
	if (code == Protocol::ANS_ACK) {
		int n = msgHnd.recInt();
		int i = 0;
		while (i < n) {
			int id = msgHnd.recInt();
			std::string title = msgHnd.recString();
			std::cout << "[" << id << "]: " << title;

			i++;
		}
	} else {
		dbgCode(Protocol::ANS_NAK, code);
		code = msgHnd.recCode(); // error-code
	}

	code = msgHnd.recCode();
	dbgCode(Protocol::ANS_END, code);
}

void Client::create_art() {
	int id;
	std::string title, author, text;

	std::cout << "article-id> "; std::cin >> id;
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
	} else {
		code = msgHnd.recCode(); // error-code
	}

	code = msgHnd.recCode();
	dbgCode(Protocol::ANS_END, code);
}

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

	/**Connection conn(argv[1], port);
	if (!conn.isConnected()) {
		std::cerr << "Connection attempt failed" << std::endl;
		exit(1);
	}

	Client client(conn);


	std::cout << client.mode << "> ";

	std::string command;
	while (std::getline(std::cin, command)) {
		auto cmd = client.cmd.find(command + "_" + client.mode);
		if (cmd == client.cmd.end()) {
			cmd = client.cmd.find(command);
			if (cmd != client.cmd.end()) 
				cmd->second;
			else 
				client.cmd_help();
		} else 
			(client.*cmd->second)();
		
		std::cout << client.mode << "> ";
	}**/
}

