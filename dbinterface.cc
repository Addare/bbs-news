#include "dbinterface.h"
using namespace std;


DBinterface::DBinterface(int port): serv(port){}

DBinterface::~DBinterface(){}


int DBinterface::start_server(){
	if(!serv.isReady())
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

int DBinterface::handle_connection(std::shared_ptr<Connection> c){
	Messagehandler mh(*c);
	int command = mh.recCode();
	switch (command){
		case Protocol::COM_LIST_NG:
			if(mh.recCode() != Protocol::COM_END){
				return 1;
			}
			vector<Newsgroup> newsgroups = db.listNewsgroups();
			mh.sendCode(Protocol::ANS_LIST_NG);
			mh.sendInt(newsgroups.size());
			for(Newsgroup& ng: newsgroups){
				mh.sendInt(ng.getId());
				mh.sendString(ng.getName());
			}
			mh.sendCode(Protocol::ANS_END);
			break;

		case Protocol::COM_CREATE_NG:
			string ng_name = mh.recString();
			if(mh.recCode() != Protocol::COM_END){
				return 1;
			}
			if(db.createNewsgroup(ng_name) != 0){
				mh.sendCode(Protocol::ANS_NAK);
				mh.sendCode(Protocol::ERR_NG_ALREADY_EXISTS);
			}else{
				mh.sendCode(Protocol::ANS_ACK)
			}
			mh.sendCode(Protocol::ANS_END);
			break;
			//add more cases
	}

	return 0;
}