#include "dbinterface.h"
using namespace std;
#include <iostream>

DBInterface::DBInterface(int port): serv(port){}

DBInterface::~DBInterface(){}


int DBInterface::startServer(){
	if(!serv.isReady())
		return 1;
	while(true){
		std::shared_ptr<Connection> c;
		try{
			c = serv.waitForActivity();
			if(c == nullptr){
				c = make_shared<Connection>();
				serv.registerConnection(c);
			}else{
				handleConnection(c);
			}
		}catch(ConnectionClosedException& e){
			serv.deregisterConnection(c);
		}
	}
	return 0;
}

int DBInterface::handleConnection(std::shared_ptr<Connection> c){
	Messagehandler mh(*c);
	int command = mh.recCode();
	cout<<"current command "<<command<<endl;;
	switch (command){
		case Protocol::COM_LIST_NG:
		 	{
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
			}

		case Protocol::COM_CREATE_NG:
			{
				string ng_name = mh.recString();
				if(mh.recCode() != Protocol::COM_END){
					return 1;
				}
				cout<<ng_name<<endl;
				mh.sendCode(Protocol::ANS_CREATE_NG);
				if(db.createNewsGroup(ng_name) != 0){
					mh.sendCode(Protocol::ANS_NAK);
					mh.sendCode(Protocol::ERR_NG_ALREADY_EXISTS);
				}else{
					cout<<"ack\n";
					mh.sendCode(Protocol::ANS_ACK);
				}
				mh.sendCode(Protocol::ANS_END);
				cout<<"success?\n";
				break;
			}

		case Protocol::COM_DELETE_NG:
			{
				int ng_id = mh.recInt();
				if(mh.recCode() != Protocol::COM_END){
					return 1;
				}
				mh.sendCode(Protocol::ANS_DELETE_NG);
				if(db.deleteNewsGroup(ng_id) != 0){
					mh.sendCode(Protocol::ANS_NAK);
					mh.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
				}else{
					mh.sendCode(Protocol::ANS_ACK);
				}
				mh.sendCode(Protocol::ANS_END);
				break;
			}

		case Protocol::COM_LIST_ART:
			{
				int ng_id = mh.recInt();
				vector<Article> v;
				if(mh.recCode() != Protocol::COM_END){
					return 1;
				}
				mh.sendCode(Protocol::ANS_LIST_ART);
				if(db.listArticles(ng_id, v) != 0){
					mh.sendCode(Protocol::ANS_NAK);
					mh.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
				}else{
					mh.sendCode(Protocol::ANS_ACK);
					mh.sendInt(v.size());
					for(int i = 0; i < v.size(); i++){
						mh.sendInt(v[i].getId());
						mh.sendString(v[i].getName());
					}
				}
				mh.sendCode(Protocol::ANS_END);
				break;
			}

		case Protocol::COM_CREATE_ART:
			{
				int id = mh.recInt();
				string title = mh.recString();
				string author = mh.recString();
				string text = mh.recString();
				if(mh.recCode() != Protocol::COM_END){
					return 1;
				}
				mh.sendCode(Protocol::ANS_CREATE_ART);
				
				if(db.createArticle(id, title, author, text)){
					mh.sendCode(Protocol::ANS_NAK);
					mh.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
				}else{
					mh.sendCode(Protocol::ANS_ACK);
				}
				mh.sendCode(Protocol::ANS_END);
				break;
			}

		case Protocol::COM_DELETE_ART:
			{
				int ng_id = mh.recInt();
				int art_id = mh.recInt();
				if(mh.recCode() != Protocol::COM_END){
					return 1;
				}
				mh.sendCode(Protocol::ANS_DELETE_ART);
				int result = db.deleteArticle(ng_id, art_id);
				if(result != 0){
					mh.sendCode(Protocol::ANS_NAK);
					if(result == 2){
						mh.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
					}else{
						mh.sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
					}
				}else{
					mh.sendCode(Protocol::ANS_ACK);
				}
				mh.sendCode(Protocol::ANS_END);
				break;
			}

		case Protocol::COM_GET_ART:
			{
				int ng_id = mh.recInt();
				int art_id = mh.recInt();
				Article a;
				if(mh.recCode() != Protocol::COM_END){
					return 1;
				}
				mh.sendCode(Protocol::ANS_GET_ART);
				int result = db.readArticle(ng_id, art_id, a);
				if(result != 0){
					mh.sendCode(Protocol::ANS_NAK);
					if(result == 2){
						mh.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
					}else{
						mh.sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
					}
				}else{
					mh.sendCode(Protocol::ANS_ACK);
					mh.sendString(a.getName());
					mh.sendString(a.getAuthor());
					mh.sendString(a.getText());
				}
				mh.sendCode(Protocol::ANS_END);
				break;
			}

		default: return 1;		//add more cases
	}

	return 0;
}