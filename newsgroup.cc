#include "newsgroup.h"
using namespace std;

Newsgroup::Newsgroup(int id, string name) : ngid(id){
	this->name = name;
	nextArtId = 0;
}

Newsgroup::~Newsgroup(){}

string Newsgroup::getName(){
	return name;
}

int Newsgroup::createArticle(string name, string author, string text){
	Article toAdd = Article(nextArtId, name, author, text);
	nextArtId++;
	articles.push_back(toAdd);
	return 0;
}

int Newsgroup::deleteArticle(int id){
	Article tempArt(id, "", "", "");
	
	auto i = remove(articles.begin(), articles.end(), tempArt);
	if(i == articles.end()){
		return 1;
	}
	articles.erase(i, articles.end());
	return 0;
}

int Newsgroup::getId(){
	return ngid;
}

vector<Article> Newsgroup::getArticles(){
	return articles;
}

bool Newsgroup::operator==(Newsgroup otherNewsgroup){
	return ngid == otherNewsgroup.ngid;
}