#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>

class Article
{
public:
	Article(int id, std::string name, std::string author, std::string text);
	~Article();
	int getId();
	std::string getName();
	std::string getAuthor();
	std::string getText();
	bool operator==(Article otherArticle);

private:
	int id;
	std::string name;
	std::string author;
	std::string text;
};
#endif