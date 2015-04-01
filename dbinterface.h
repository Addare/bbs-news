#ifndef DBINTERFACE_H
#define DBINTERFACE_H
#include <string>
#include <stdexcept>
#include "connectionclosedexception.h"
#include "server.h"
#include "discdatabase.h"
#include "newsgroup.h"
#include "protocol.h"


class DBInterface{
public:
	DBInterface(int port);
	~DBInterface();
	int startServer();
private:
	Server serv;
	DiscDatabase db;
	int handleConnection(std::shared_ptr<Connection> c);
};




#endif