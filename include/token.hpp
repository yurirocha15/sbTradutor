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
	void setType(std::string type);
	std::string getType();
	void setLine(int line);
	int getLine();
	void setOp(std::string op);
	std::string getOp();
private:
	std::string name;
	std::string type;
	std::string op;
	int line;
};

#endif //_TOKEN_HPP_