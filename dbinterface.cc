#include "dbinterface.h"
using namespace std;


DBinterface::DBinterface(int port): server(port){}

DBinterface::~DBinterface(){}


int DBinterface::start_server(){
	if(!serv.isReady)
		return 1;
	while(true){
		std::shared_ptr<Connection> c = serv.waitForActivity();
		if(c == nullptr){
			c = make_shared<Connection>();
			serv.registerConnection(c);
		}else{
			handle_connection(c);
		}
	}
	return 0;
}

int handle_connection(std::shared_ptr<Connection> c){
	Messagehandler mh(*c);
	int command = mh.recCode();

	switch (command){
		case Protocol.COM_LIST_NG:
			if(mh.recCode != Protocol.COM_END){
				return 1;
			}
			vector<Newsgroup> newsgroups = db.listNewsGroups();
			mh.sendCode(Protocol.ANS_LIST_NG);
			mh.sendInt(newsgroups.size());
			for(Newsgroup& ng: newsgroups){
				mh.sendInt(ng.getId);
				mh.sendString(ng.getName());
			}
			mh.sendCode(Protocol.ANS_END)
	}

	return 0;
}