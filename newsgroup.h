#ifndef NEWSGROUP_H
#define NEWSGROUP_H
#include <vector>
#include <string>
#include <algorithm>
#include "article.h"

class Newsgroup{
public:
	Newsgroup(int id, std::string name);
	~Newsgroup();
	int createArticle(std::string name, std::string author, std::string text);
	int createArticle(int artcleid, std::string name, std::string author, std::string text);
	int deleteArticle(int id);
	std::string getName();
	int getId();
	std::vector<Article> getArticles();
	bool operator==(Newsgroup otherNewsgroup);
private:
	std::string name;
	int ngid;
	int nextArtId;
	std::vector<Article> articles;
};
#endif