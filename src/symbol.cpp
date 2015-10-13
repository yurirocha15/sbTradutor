#include <symbol.hpp>
#include <verboseLog.hpp>

#include <iostream>

using namespace std;

Symbol::Symbol(std::string name, int line, int adress)
{
	this->name = name;
	this->line = line;
	this->adress = adress;
}

Symbol::~Symbol()
{

}

void Symbol::setName(std::string name)
{
	this->name = name;
}

string Symbol::getName()
{
	return name;
}

void Symbol::setLine(int line)
{
	this->line = line;
}

int Symbol::getLine()
{
	return line;
}

void Symbol::setAdress(int adress)
{
	this->adress = adress;
}

int Symbol::getAdress()
{
	return adress;
}