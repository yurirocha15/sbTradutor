#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <string>
#include <vector>
#include <symbol.hpp>

class Parser
{
public:
	Parser(std::vector<std::string> lineVector, std::vector<Symbol> *labelTable, std::vector<Token> *tokenList);
	virtual ~Parser();
private:
	void firstPass(std::vector<std::string> lineVector, std::vector<Symbol> *labelTable);
	void secondPass(std::vector<std::string> lineVector, std::vector<Symbol> *labelTable, std::vector<Token> *tokenList)
};

#endif //_PARSER_HPP_
