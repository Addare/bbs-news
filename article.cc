#include "article.h"
using namespace std;


Article::Article(){}

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