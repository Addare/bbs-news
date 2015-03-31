#ifndef DBINTERFACE_H
#define DBINTERFACE_H
#include <string>
#include "server.h"
#include "database.h"
#include "newsgroup.h"
#include "protocol.h"


class DBinterface{
public:
	DBinterface(int port);
	~DBinterface();
	int start_server();
private:
	Server serv;
	Database db;
	int handle_connection(std::shared_ptr<Connection> c);
};




#endif