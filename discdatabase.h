#ifndef DISCDATABASE_H
#define DISCDATABASE_H
#include "server.h"
#include "newsgroup.h"
#include "article.h"
#include "messagehandler.h"
#include "database.h"
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstdio>

class DiscDatabase : public Database{
public:
	DiscDatabase(std::string databasepath);
	~DiscDatabase();
	int createNewsGroup(std::string name);
	int createNewsGroup(int newsgroupid, std::string name);
	int deleteNewsGroup(int newsgroupid);
	int createArticle(int newsgroupid, std::string name, std::string author, std::string text);
	int createArticle(int newsgroupid, int articleid, std::string name, std::string author, std::string text);
	int deleteArticle(int newsgroupid, int articleid);
private:
	void writeNgToFile(int newsgroupid, std::string newsgroupname);
	void addArtToNgFile(int newsgroupid, std::string newsgroupname, Article toadd);
	void writeArticleToFile(Article toWrite);
	int deleteArticleOnFile(int newsgroupid, int articleid);
	int restoreFromFile();
	int deleteNewsGroupOnFile(int newsgroupid);
	std::ofstream toIndex;
	std::string path;
};
#endif