#include "database.h"
using namespace std;

Database::Database(){

}
Database::~Database(){}

int Database::createNewsGroup(string name){
	Newsgroup ng = Newsgroup(nextngid, name);
	auto found = find_if(newsgroups.begin(), newsgroups.end(),[name](Newsgroup n){return n.getName() == name;});
	if(found!=newsgroups.end()){
		return 1;
	}
	nextngid++;
	newsgroups.push_back(ng);
	return 0;
}

int Database::deleteNewsGroup(int newsgroupid){
	Newsgroup tempNg(newsgroupid, "");
	auto i = remove(newsgroups.begin(), newsgroups.end(), tempNg);
	if(i == newsgroups.end()){
		return 1;
	}
	newsgroups.erase(i, newsgroups.end());
	return 0;
}

int Database::createArticle(int newsgroupid, std::string name, std::string author, std::string text){
	Newsgroup temp = Newsgroup(newsgroupid, "");
	auto ng = find(newsgroups.begin(), newsgroups.end(), temp);
	if(ng == newsgroups.end()){
		return 1;
	}
	ng->createArticle(name, author, text);
	return 0;
}

int Database::deleteArticle(int newsgroupid, int articleid){
	Newsgroup temp = Newsgroup(newsgroupid, "");
	auto ng = find(newsgroups.begin(), newsgroups.end(), temp);
	if(ng == newsgroups.end()){
		return 2;
	}
	return ng->deleteArticle(articleid);
}

vector<Newsgroup> Database::listNewsgroups(){
	return newsgroups;
}

int Database::listArticles(int newsgroupid, vector<Article>& v){
	Newsgroup temp = Newsgroup(newsgroupid, "");
	auto ng = find(newsgroups.begin(), newsgroups.end(), temp);
	if(ng == newsgroups.end()){
		return 1;
	}
	v = ng->getArticles();
	return 0;
}
	
int Database::readArticle(int newsgroupid, int articleid, Article& a){
	Newsgroup temp = Newsgroup(newsgroupid, "");
	auto ng = find(newsgroups.begin(), newsgroups.end(), temp);
	if(ng == newsgroups.end()){
		return 2;
	}
	Article tempArt(articleid, "", "", "");
	vector<Article> newsgroup = ng->getArticles();
	auto art = find(newsgroup.begin(), newsgroup.end(), tempArt);
	if(art == newsgroup.end()){
		return 1;
	}
	a = (*art);
	return 0;
}