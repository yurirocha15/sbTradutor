#ifndef _TOKEN_HPP_
#define _TOKEN_HPP_

#include <string>
#include <vector>

class Token
{
public:
	Token(std::string name, int line);
	virtual ~Token();

	void setName(std::string name);
	std::string getName();
	void setSpace_const(std::string space_const);
	std::string getSpace_const();
	void setType(std::string type);
	std::string getType();
	void setLine(int line);
	int getLine();
	void setOp(std::string op);
	std::string getOp();
	void setSize(int size);
	int getSize();
private:
	std::string name;
	std::string type;
	std::string op;
	std::string space_const;
	int line;
	int size;
};

#endif //_TOKEN_HPP_