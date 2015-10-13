#include <token.hpp>
#include <verboseLog.hpp>

#include <iostream>

using namespace std;

Token::Token(std::string name, int line)
{
	this->name = name;
	this->line = line;
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

void Token::setLine(int line)
{
	this->line = line;
}

int Token::getLine()
{
	return line;
}

void Token::setOp(string op)
{
	this->op = op;
}

string Token::getOp()
{
	return op;
}