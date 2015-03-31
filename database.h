#ifndef DATABASE_H
#define DATABASE_H
#include "server.h"
#include "newsgroup.h"
#include "article.h"
#include "messagehandler.h"
#include <vector>

class Database{
public:
	Database();
	~Database();
	int createNewsGroup(std::string name);
	int deleteNewsGroup(int newsgroupid);
	int createArticle(int newsgroupid, std::string name, std::string author, std::string text);
	int deleteArticle(int newsgroupid, int articleid);
	std::vector<std::string> listNewsgroups();
	std::vector<std::string> listArticles(int newsgroupid);
	std::string readArticle(int newsgroupid, int articleid);
private:
	int nextngid;
	Server server;
	std::vector<Newsgroup> newsgroups;

};
#endif