#include <montador.hpp>
#include <token.hpp>
#include <symbol.hpp>
#include <parser.hpp>
#include <verboseLog.hpp>

#include <iostream>

using namespace std;

Montador::Montador(const std::string& inputFile, const std::string& outputFile)
{
	std::vector<Symbol> labelTable;
	std::vector<Token> tokenList;
	Parser parser(inputFile, outputFile, labelTable, tokenList);
}

Montador::~Montador()
{
	
}