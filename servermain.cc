#include "dbinterface.h"
#include <iostream>
using namespace std;



int main(int argc,char *argv[]){
	int port = 4242;
	if(argc > 2){
		cerr<<"Too many arguments. Only enter port";
		return 1;
	}
	if(argc == 2){
		port = static_cast<int>(*argv[1]);
	}
	DBInterface db(port);
	db.startServer();
	return 0;
}