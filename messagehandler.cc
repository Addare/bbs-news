#include "messagehandler.h"
using namespace std;


Messagehandler::Messagehandler(Connection c): conn(c){}

Messagehandler::~Messagehandler(){}

Messagehandler::sendCode(int code){
	conn.write(static_cast<unsigned char>(code));
}

Messagehandler::sendInt(int code){
	conn.write(static_cast<unsigned char>(code>>24 & 0xFF));
	conn.write(static_cast<unsigned char>(code>>16 & 0xFF));
	conn.write(static_cast<unsigned char>(code>>8 & 0xFF));
	conn.write(static_cast<unsigned char>(code & 0xFF));
}