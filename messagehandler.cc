#include "messagehandler.h"
#include <iostream>
using namespace std;


Messagehandler::Messagehandler(Connection& c): conn(c){}

Messagehandler::~Messagehandler(){}

void Messagehandler::sendCode(int code){
	conn.write(static_cast<unsigned char>(code));
}

void Messagehandler::sendInt(int value, bool paramNum){
	if(paramNum){
		sendCode(Protocol::PAR_NUM);
	}
	conn.write(static_cast<unsigned char>(value>>24 & 0xFF));
	conn.write(static_cast<unsigned char>(value>>16 & 0xFF));
	conn.write(static_cast<unsigned char>(value>>8 & 0xFF));
	conn.write(static_cast<unsigned char>(value & 0xFF));
}

void Messagehandler::sendString(string s){
	sendCode(Protocol::PAR_STRING);
	sendInt(s.length(), false);
	for(char& c: s){
		conn.write(c);
	}
}

int Messagehandler::recCode(){
	return static_cast<int>(conn.read());
}

int Messagehandler::recInt(bool check){
	if(check && recCode() != Protocol::PAR_NUM){
			cerr<<"not an int\n";
	}
	return (static_cast<int>(conn.read())<<24) + (static_cast<int>(conn.read()<<16))
			 + (static_cast<int>(conn.read())<<8) + static_cast<int>(conn.read());
}

string Messagehandler::recString(){
	if(recCode() != Protocol::PAR_STRING){
			cerr<<"not a string\n";
	}
	string s;
	int length = recInt(false);
	for(int i = 0; i < length; i++){
		s += conn.read();
	}
	return s;
}