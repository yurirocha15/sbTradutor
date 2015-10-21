#include <token.hpp>
#include <verboseLog.hpp>

#include <iostream>

using namespace std;

Token::Token(std::string name, int line)
{
	this->name = name;
	this->line = line;
	size = 999;
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

void Token::setSpace_const(std::string space_const)
{
	this->space_const = space_const;
}

string Token::getSpace_const()
{
	return space_const;
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

void Token::setSize(int size)
{
	this->size = size;
}

int Token::getSize()
{
	return size;
}