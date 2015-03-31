#include "messagehandler.h"
using namespace std;


Messagehandler::Messagehandler(Connection& c): conn(c){}

Messagehandler::~Messagehandler(){}

void Messagehandler::sendCode(int code){
	conn.write(static_cast<unsigned char>(code));
}

void Messagehandler::sendInt(int value){
	sendCode(Protocol::PAR_NUM);
	conn.write(static_cast<unsigned char>(value>>24 & 0xFF));
	conn.write(static_cast<unsigned char>(value>>16 & 0xFF));
	conn.write(static_cast<unsigned char>(value>>8 & 0xFF));
	conn.write(static_cast<unsigned char>(value & 0xFF));
}

void Messagehandler::sendString(string s){
	sendCode(Protocol::PAR_STRING);
	sendInt(s.length());
	for(char& c: s){
		conn.write(c);
	}
}

int Messagehandler::recCode(){
	return static_cast<int>(conn.read());
}

int Messagehandler::recInt(){
	if(recCode() != Protocol::PAR_NUM){
			//error
	}
	return (static_cast<int>(conn.read())<<24) + (static_cast<int>(conn.read()<<16))
			 + (static_cast<int>(conn.read())<<8) + static_cast<int>(conn.read());
}

string Messagehandler::recString(){
	if(recCode() != Protocol::PAR_STRING){
			//error
	}
	string s;
	int length = static_cast<int>(recInt());
	for(int i = 0; i < length; i++){
		s += conn.read();
	}
	return s;
}