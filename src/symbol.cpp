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

void Symbol::setSpace_const(std::string space_const)
{
	this->space_const = space_const;
}

string Symbol::getSpace_const()
{
	return space_const;
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

void Symbol::setSize(int size)
{
	this->size = size;
}

int Symbol::getSize()
{
	return size;
}