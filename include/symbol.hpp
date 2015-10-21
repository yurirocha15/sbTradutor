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
	void setSpace_const(std::string space_const);
	std::string getSpace_const();
	void setLine(int line);
	int getLine();
	void setAdress(int adress);
	int getAdress();
	void setSize(int size);
	int getSize();
private:
	std::string name;
	std::string space_const;//indica se o símbolo é um SPACE ou CONST
	int line;
	int adress;
	int size;
};

#endif //_SYMBOL_HPP_