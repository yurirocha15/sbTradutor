#include <token.hpp>
#include <verboseLog.hpp>

#include <iostream>

using namespace std;

Token::Token(std::string name, std::string type, int adress)
{
	this->name = name;
	this->type = type;
	this->adress = adress;
}

Token::~Token()
{

}

void Token::setName(std::string name)
{
	this->name = name;
}

string Token::getName()
{
	return name;
}

void Token::setType(std::string type)
{
	this->type = type;
}

string Token::getType()
{
	return type;
}

void Token::setAdress(int adress)
{
	this->adress = adress;
}

Token::getAdress()
{
	return adress;
}