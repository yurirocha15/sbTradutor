#include <symbol.hpp>
#include <verboseLog.hpp>

#include <iostream>

using namespace std;

Symbol::Symbol(std::string name, int adress)
{
	this->name = name;
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

void Symbol::setAdress(int adress)
{
	this->adress = adress;
}

Symbol::getAdress()
{
	return adress;
}