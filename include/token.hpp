#ifndef _TOKEN_HPP_
#define _TOKEN_HPP_

#include <string>
#include <vector>

class Token
{
public:
	Token(std::string name, std::string type, int adress);
	virtual ~Token();

	void setName(std::string name);
	std::string getName();
	void setType(std::string type);
	std::string getType();
	void setAdress(int adress);
	int getAdress();
private:
	std::string name;
	std::string type;
	int adress;


};

#endif //_TOKEN_HPP_