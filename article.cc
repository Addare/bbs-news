#include "article.h"
using namespace std;


Article::Article(){}

Article::Article(const Article& rhs){
	id = rhs.id;
 	name = rhs.name;
 	author = rhs.author;
 	text = rhs.text;
 }

Article::Article(int id, string name, string author, string text){
	this->id = id;
 	this->name = name;
 	this->author = author;
 	this->text = text;
 }

Article::~Article(){}


int Article::getId(){
	return id;
}

string Article::getName(){
	return name;
}

string Article::getAuthor(){
	return author;
}

string Article::getText(){
	return text;
}

bool Article::operator==(Article otherArticle){
	return id == otherArticle.id;
}

Article& Article::operator=(const Article& rhs){
	id = rhs.id;
	name = rhs.name;
	author = rhs.author;
	text = rhs.text;
	return *this;
}