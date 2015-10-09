#include <parser.hpp>
#include <verboseLog.hpp>
#include <boost/algorithm/string.hpp>


#include <iostream>

using namespace std;

Parser::Parser(std::vector<std::string> lineVector, std::vector<Symbol> *labelTable, std::vector<Token> *tokenList)
{
	this->firstPass(lineVector, labelTable);
}

Parser::~Parser()
{

}

void Parser::firstPass(std::vector<std::string> lineVector, std::vector<Symbol> *labelTable)
{
	//método responsável em executar a primeira passagem, ou seja, irá percorrer todas as linhas, e quanto achar labels 
	//(tokens terminados em ":") adiciona-lo a tabela de simbolos, juntamente com seu endereço

	unsigned int i,j; //contadores
	std::vector<string> tokensLine;
	char last;
	string labelName;

	for(i = 0; i < lineVector.size(); i++)
	{
		boost::split(tokensLine,lineVector[i],boost::is_any_of(" "));
		for(j = 0;j < tokensLine.size();j++)
		{
			last = tokensLine[j].back();
			if(last == ':' )
			{
				labelName = tokensLine[j].substr(0,tokensLine[j].size()-1);
				Symbol label(labelName,i+1);
				labelTable->push_back(label);
			}
		}
		tokensLine.erase(tokensLine.begin(),tokensLine.end());
	}
}

void Parser::secondPass(std::vector<std::string> lineVector, std::vector<Symbol> *labelTable, std::vector<Token> *tokenList)
{
	//A segunda Passagem irá classificar todos os elementos em Line Vector, simbolo, instrução e diretiva;
}