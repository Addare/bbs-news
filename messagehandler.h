#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include "connection.h"

class Messagehandler
{
public:
	Messagehandler(Connection c);
	~Messagehandler();

	void sendCode(int code);
	void sendInt(int value);
	void sendString(std::string text);

	int recCode();
	int recInt();
	std::string recString();

private:
	Connection conn;
};


#endif