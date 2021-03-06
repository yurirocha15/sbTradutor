#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <string>
#include <vector>
#include <symbol.hpp>
#include <token.hpp>

class Parser
{
public:
	Parser(const std::string& inputFile, const std::string& outputFile, std::vector<Symbol>& labelTable, std::vector<Token>& tokenList);
	virtual ~Parser();
private:
	std::vector<Token> firstPass(std::vector<std::string> lineVector, std::vector<Symbol>& labelTable);
	void secondPass(std::vector<Symbol>& labelTable, std::vector<Token>& tokenList);
	void isLabel(std::vector<Symbol>& labelTable, std::vector<Token>& tokenList);
	void isDirective(std::vector<Token>& tokenList);
	void isInstruction(std::vector<Token>& tokenList);
	void detectError(std::vector<Symbol>& labelTable, std::vector<Token> tokenList);

	std::string inputFile;
	std::string outputFile;
	bool hasError;
};

#endif //_PARSER_HPP_
