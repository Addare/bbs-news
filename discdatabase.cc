#include "discdatabase.h"
#include <iostream>

using namespace std;

DiscDatabase::DiscDatabase(string databasepath) : path(databasepath){
	restoreFromFile();
	nextngid = 0;
}

DiscDatabase::~DiscDatabase(){
}

//TODO: Check function
int DiscDatabase::restoreFromFile(){
//	cout<<"banankontakt\n";
	ifstream index;
	index.open(path + "/index");
  	if (index.is_open())
  	{	
  		string indexline;
    	while ( getline (index,indexline) )
    	{
    		size_t foundveryfirst = indexline.find_first_of(";");
    		size_t foundfirstdot = indexline.find_last_of(".");
    		int newsgroupid = stoi(indexline.substr(foundveryfirst+1, foundveryfirst-foundfirstdot-1));
    		string newsgroupname = indexline.substr(1, foundveryfirst-1);
    		createNewsGroup(newsgroupid, newsgroupname);
      		ifstream ngFile;
      		ngFile.open(path + indexline);
  			if (ngFile.is_open())
  			{
  				string ngline;
    			while ( getline (ngFile,ngline) )
    			{
    				vector<Article> newsgrouparticles;
      				size_t foundfirst = ngline.find_first_of(";");
      				size_t foundlast = ngline.find_last_of(";");
      				string articleid = ngline.substr(0, foundfirst);
      				string articlename = ngline.substr(foundfirst+1, foundlast-1-foundfirst);
      				string articleauthor = ngline.substr(foundlast+1);
      				string articletext;
      				ifstream articleFile;
      				articleFile.open(path + "/" + articlename + ";" + articleid + ";" + articleauthor + ".art");
      				string articleline;
      				while(getline(articleFile, articleline)){
      					articletext += articleline + "\n";
      				}
      				articleFile.close();
      				createArticle(newsgroupid, stoi(articleid), articlename, articleauthor, articletext);
    			}
    		ngFile.close();
  			}
    	}
    index.close();
  	}
  	return 0;
}
//overloadaed to take int id
int DiscDatabase::createNewsGroup(int id, string name){
	Newsgroup ng(id, name);
	auto found = find_if(newsgroups.begin(), newsgroups.end(),[name](Newsgroup n){return n.getName() == name;});
	if(found!=newsgroups.end()){
		return 1;
	}
	newsgroups.push_back(ng);
	if(id > nextngid){
		 nextngid = id + 1;
	}
	return 0;
}

int DiscDatabase::createNewsGroup(string name){
	Newsgroup ng(nextngid, name);
	auto found = find_if(newsgroups.begin(), newsgroups.end(),[name](Newsgroup n){return n.getName() == name;});
	if(found!=newsgroups.end()){
		return 1;
	}
	newsgroups.push_back(ng);
	writeNgToFile(nextngid, name);
	nextngid++;
	return 0;
}

void DiscDatabase::writeNgToFile(int newsgroupid, string newsgroupname){
	ofstream ngFile;
  	ngFile.open (path + "/" + newsgroupname + ";" + to_string(newsgroupid) + ".ng");
 	ngFile.close();
	toIndex.open(path + "/index", ofstream::app);
  	toIndex << "/" << newsgroupname << ";" << newsgroupid << ".ng" << "\n";
  	toIndex.close();
}

void DiscDatabase::addArtToNgFile(int newsgroupid, string newsgroupname , Article toadd){
	ofstream ngFile;
	ngFile.open (path + "/" + newsgroupname + ";" + to_string(newsgroupid) + ".ng", ofstream::app);
	ngFile << toadd.getId() << ";" << toadd.getName() << ";" << toadd.getAuthor() << "\n";
	ngFile.close();
}

int DiscDatabase::deleteNewsGroup(int newsgroupid){
	Newsgroup tempNg(newsgroupid, "");
	auto i = remove(newsgroups.begin(), newsgroups.end(), tempNg);
	if(i == newsgroups.end()){
		return 1;
	}
	deleteNewsGroupOnFile(newsgroupid);
	newsgroups.erase(i, newsgroups.end());
	return 0;
}


//TODO: Remove ng from index
int DiscDatabase::deleteNewsGroupOnFile(int newsgroupid){
	//Delete all articles in newsgroup
	Newsgroup temp = Newsgroup(newsgroupid, "");
	auto ng = find(newsgroups.begin(), newsgroups.end(), temp);
	if(ng == newsgroups.end()){
		return 2;
	}
	vector<string> newfile;
	string ngline;
	ifstream ngFile (path + "/" + ng->getName() + ";" + to_string(ng->getId()) + ".ng");
  	if (ngFile.is_open())
  	{
    	while ( getline (ngFile,ngline) )
    	{
      		size_t found = ngline.find_first_of(";");
      		string id = ngline.substr(0, found);
      		deleteArticleOnFile(newsgroupid, stoi(id));
    	}
    ngFile.close();
  	}
  	if(remove((path + "/" + ng->getName() + ";" + to_string(ng->getId()) + ".ng").c_str())){
  		return 1;
  	}
	return 0;
}

int DiscDatabase::createArticle(int newsgroupid, int articleid, string name, string author, string text){
	Newsgroup temp(newsgroupid, "");
	auto ng = find(newsgroups.begin(), newsgroups.end(), temp);
	if(ng == newsgroups.end()){
		return 1;
	}
	ng->createArticle(articleid, name, author, text);
	return 0;
}


int DiscDatabase::createArticle(int newsgroupid, string name, string author, string text){
	Newsgroup temp(newsgroupid, "");
	auto ng = find(newsgroups.begin(), newsgroups.end(), temp);
	if(ng == newsgroups.end()){
		return 1;
	}
	int id  = ng->createArticle(name, author, text);
	addArtToNgFile(newsgroupid, ng->getName() , Article(id, name, author, text));
	writeArticleToFile(Article(id, name, author, text));
	return 0;
}

void DiscDatabase::writeArticleToFile(Article toWrite){
	ofstream ngFile;
  	ngFile.open (path + "/" + toWrite.getName() + ";" + to_string(toWrite.getId()) + ";" + toWrite.getAuthor() + ".art");
  	ngFile << toWrite.getText();
 	ngFile.close();
}

int DiscDatabase::deleteArticle(int newsgroupid, int articleid){
	Newsgroup temp = Newsgroup(newsgroupid, "");
	auto ng = find(newsgroups.begin(), newsgroups.end(), temp);
	if(ng == newsgroups.end()){
		return 2;
	}
	deleteArticleOnFile(newsgroupid, articleid);
	return ng->deleteArticle(articleid);
}

int DiscDatabase::deleteArticleOnFile(int newsgroupid, int articleid){
	Newsgroup temp(newsgroupid, "");
	auto ng = find(newsgroups.begin(), newsgroups.end(), temp);
	if(ng == newsgroups.end()){
		return 2;
	}
	vector<string> newfile;
	string line;
	ifstream ngFile(path + "/" + ng->getName() + ";" + to_string(ng->getId()) + ".ng");
  	if(ngFile.is_open())
  	{
    	while(getline(ngFile,line))
    	{
      		size_t found = line.find_first_of(";");
      		string id = line.substr(0, found);
      		cerr << id;
      		if(stoi(id) != articleid){
      			newfile.push_back(line);
      		}
    	}
    ngFile.close();
  	}
  	//remove((path + "/" + ng->getName() + ";" + to_string(ng->getId()) + ".ng").c_str());
  	ofstream newNgFile;
  	newNgFile.open (path + "/" + ng->getName() + ";" + to_string(ng->getId()) + ".ng");
  	for(size_t i = 0; i < newfile.size(); i++){
		newNgFile << newfile[i] << "\n";
	}
	newNgFile.close();
	vector<Article> newsgrouparticles;
	newsgrouparticles = ng->getArticles();
	Article temp2(articleid, "", "", "");
	auto arttoremove = find(newsgrouparticles.begin(), newsgrouparticles.end(), temp2);
	if(arttoremove != newsgrouparticles.end()){
		remove((path + "/" + arttoremove->getName() + ";" + to_string(arttoremove->getId()) + ";" + arttoremove->getAuthor() + ".art").c_str());
	}
	else{
		return 3;
	}
	return 0;
}

vector<Newsgroup> DiscDatabase::listNewsgroups(){
	return newsgroups;
}

int DiscDatabase::listArticles(int newsgroupid, vector<Article>& v){
	Newsgroup temp = Newsgroup(newsgroupid, "");
	auto ng = find(newsgroups.begin(), newsgroups.end(), temp);
	if(ng == newsgroups.end()){
		return 1;
	}
	v = ng->getArticles();
	return 0;
}
	
int DiscDatabase::readArticle(int newsgroupid, int articleid, Article& a){
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