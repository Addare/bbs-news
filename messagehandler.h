#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include <string>
#include "connection.h"
#include "protocol.h"


class Messagehandler
{
public:
	Messagehandler(Connection& c);
	~Messagehandler();

	void sendCode(int code);
	void sendInt(int value, bool paramNum = true);
	void sendString(std::string text);

	int recCode();
	int recInt(bool check = true);
	std::string recString();

private:
	Connection& conn;
};


#endif