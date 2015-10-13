#ifndef _SYMBOL_HPP_
#define _SYMBOL_HPP_

#include <string>
#include <vector>

class Symbol
{
public:
	Symbol(std::string name, int line, int adress);
	virtual ~Symbol();

	void setName(std::string name);
	std::string getName();
	void setLine(int line);
	int getLine();
	void setAdress(int adress);
	int getAdress();
private:
	std::string name;
	int line;
	int adress;

};

#endif //_SYMBOL_HPP_