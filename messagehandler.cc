#include "messagehandler.h"
using namespace std;


Messagehandler::Messagehandler(Connection c): conn(c){}

Messagehandler::~Messagehandler(){}

void Messagehandler::sendCode(int code){
	conn.write(static_cast<unsigned char>(code));
}

void Messagehandler::sendInt(int code){
	conn.write(static_cast<unsigned char>(code>>24 & 0xFF));
	conn.write(static_cast<unsigned char>(code>>16 & 0xFF));
	conn.write(static_cast<unsigned char>(code>>8 & 0xFF));
	conn.write(static_cast<unsigned char>(code & 0xFF));
}

void Messagehandler::sendString(string s){
	for(char& c: s){
		conn.write(c);
	}
}

int Messagehandler::recCode(){
	return static_cast<int>(conn.read());
}

int Messagehandler::recInt(){
	return static_cast<int>(conn.read())<<24 + static_cast<int>(conn.read()<<16)
			 + static_cast<int>(conn.read())<<8 + static_cast<int>(conn.read());
}

int Messagehandler::recString(){
	string s;
	int length = static_cast<int>(conn.read());
	for(int i = 0; i < length; i++){
		s += conn.read();
	}
	return s;
}