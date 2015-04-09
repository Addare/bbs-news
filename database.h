#ifndef DATABASE_H
#define DATABASE_H
#include "server.h"
#include "newsgroup.h"
#include "article.h"
#include "messagehandler.h"
#include <vector>
#include <string>
#include <algorithm>

class Database{
public:
	Database();
	virtual ~Database();
	virtual int createNewsGroup(std::string name);
	virtual int deleteNewsGroup(int newsgroupid);
	virtual int createArticle(int newsgroupid, std::string name, std::string author, std::string text);
	virtual int deleteArticle(int newsgroupid, int articleid);
	std::vector<Newsgroup> listNewsgroups();
	int listArticles(int newsgroupid, std::vector<Article>& v);
	int readArticle(int newsgroupid, int articleid, Article& a);
protected:
	int nextngid;
	std::vector<Newsgroup> newsgroups;

};
#endif