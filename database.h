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
	vector<Newsgroup> listNewsgroups();
	vector<Article> listArticles(int newsgroupid);
	Article readArticle(int newsgroupid, int articleid);
private:
	int nextngid;
	std::vector<Newsgroup> newsgroups;

};
#endif