#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>

class Article
{
public:
	Article();
	Article(const Article& rhs);
	Article(int id, std::string name, std::string author, std::string text);
	~Article();
	int getId();
	std::string getName();
	std::string getAuthor();
	std::string getText();
	bool operator==(Article otherArticle);
	Article& operator=(const Article& rhs);

private:
	int id;
	std::string name;
	std::string author;
	std::string text;
};

#endif