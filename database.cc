#include "database.h"
using namespace std;

Database::Database(){

}

int Database::createNewsGroup(std::string name){
	Newsgroup ng = Newsgroup(nextngid, string name);
	auto found = find_if(newsgroups.begin(), newsgroups.end(),[name](Newsgroup n){return n.getName() == name});
	if(found!=newsgroups.end()){
		return 1;
	}
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

vector<Article> Database::listArticles(int newsgroupid){
	Newsgroup temp = Newsgroup(newsgroupid, "");
	auto ng = find(newsgroups.begin(), newsgroups.end(), temp);
	if(ng == newsgroups.end()){
		vector<Article> empty;
		return empty;
	}
	return ng->getArticles();
}
	
Article Database::readArticle(int newsgroupid, int articleid){
	Newsgroup temp = Newsgroup(newsgroupid, "");
	auto ng = find(newsgroups.begin(), newsgroups.end(), temp);
	if(ng == newsgroups.end()){
		return "";
	}
	Article tempArt(articleid, "", "", "");
	auto art = find(ng.begin(), ng.end(), tempArt);
	if(art == ng.end()){
		return "";
	}
	return art;
}