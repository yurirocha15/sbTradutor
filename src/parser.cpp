#include <parser.hpp>
#include <verboseLog.hpp>
#include <macro.hpp>
#include <boost/algorithm/string.hpp>


#include <iostream>
#include <cstdlib>

using namespace std;

Parser::Parser(const std::string& inputFile, const std::string& outputFile, std::vector<Symbol>& labelTable, std::vector<Token>& tokenList)
{
	vector<string> lineVector;
	Macro scanner(inputFile,outputFile);
	lineVector = scanner.GetLineVector();
	vector<Token> listaTokens = this->firstPass(lineVector, labelTable);
}

Parser::~Parser()
{

}

std::vector<Token> Parser::firstPass(std::vector<std::string> lineVector, std::vector<Symbol>& labelTable)
{
	//método responsável em executar a primeira passagem, ou seja, irá percorrer todas as linhas, e quanto achar labels 
	//(tokens terminados em ":") adiciona-lo a tabela de simbolos, juntamente com seu endereço

	unsigned int i,j; //contadores
	int adress = 0;
	vector<string> tokensLine;
	std::vector<Token> tokenList;
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
				Token token(labelName,i+1);
				token.setType("LABEL");
				tokenList.push_back(token);
				//---Transformar int em String---
				
				//-------------------------------
				Symbol label(labelName,i+1,adress);
				labelTable.push_back(label);
			}
			else
			{
				Token token(tokensLine[j],i+1);
				tokenList.push_back(token);
				adress++;
			}
		}
		tokensLine.erase(tokensLine.begin(),tokensLine.end());
	}
	return tokenList;
}

void Parser::secondPass(std::vector<Symbol>& labelTable, std::vector<Token>& tokenList)
{
	//A segunda Passagem irá classificar todos os elementos em Line Vector, simbolo, INSTRUÇÃO e diretiva;
	this->isInstruction(tokenList);
	this->isDirective(tokenList);
	this->isLabel(labelTable,tokenList);
}

void Parser::isInstruction(std::vector<Token>& tokenList)
{
	unsigned int i;
	for(i = 0; i < tokenList.size(); i++)//Laço compara o token com todas as instruções conhecidas, caso positivo, classifica o typo
	{									//Do token como INSTRUÇÃO e atribui seu OPCODE
		if(tokenList[i].getType().empty())//Entra se o type for NULL
		{								  
			if(tokenList[i].getName() == "ADD")
			{
				tokenList[i].setType("INSTRUÇÃO");
				tokenList[i].setOp("01");
			}
			else if(tokenList[i].getName() == "SUB")
			{
				tokenList[i].setType("INSTRUÇÃO");
				tokenList[i].setOp("02");
			}
			else if(tokenList[i].getName() == "MULT")
			{
				tokenList[i].setType("INSTRUÇÃO");
				tokenList[i].setOp("03");
			}
			else if(tokenList[i].getName() == "DIV")
			{
				tokenList[i].setType("INSTRUÇÃO");
				tokenList[i].setOp("04");
			}
			else if(tokenList[i].getName() == "JMP")
			{
				tokenList[i].setType("INSTRUÇÃO");
				tokenList[i].setOp("05");
			}
			else if(tokenList[i].getName() == "JMPN")
			{
				tokenList[i].setType("INSTRUÇÃO");
				tokenList[i].setOp("06");
			}
			else if(tokenList[i].getName() == "JMPP")
			{
				tokenList[i].setType("INSTRUÇÃO");
				tokenList[i].setOp("07");
			}
			else if(tokenList[i].getName() == "JMPZ")
			{
				tokenList[i].setType("INSTRUÇÃO");
				tokenList[i].setOp("08");
			}
			else if(tokenList[i].getName() == "COPY")
			{
				tokenList[i].setType("INSTRUÇÃO");
				tokenList[i].setOp("09");
			}
			else if(tokenList[i].getName() == "LOAD")
			{
				tokenList[i].setType("INSTRUÇÃO");
				tokenList[i].setOp("10");
			}
			else if(tokenList[i].getName() == "STORE")
			{
				tokenList[i].setType("INSTRUÇÃO");
				tokenList[i].setOp("11");
			}
			else if(tokenList[i].getName() == "INPUT")
			{
				tokenList[i].setType("INSTRUÇÃO");
				tokenList[i].setOp("12");
			}
			else if(tokenList[i].getName() == "OUTPUT")
			{
				tokenList[i].setType("INSTRUÇÃO");
				tokenList[i].setOp("13");
			}
			else if(tokenList[i].getName() == "STOP")
			{
				tokenList[i].setType("INSTRUÇÃO");
				tokenList[i].setOp("14");
			}
		}
			
	}
}

void Parser::isDirective(vector<Token>& tokenList)
{
	unsigned int i;
	for(i = 0; i < tokenList.size(); i++)
	{
		if(tokenList[i].getType().empty())
		{
			if(tokenList[i].getName() == "SECTION")
			{
				tokenList[i].setType("DIRETIVA");
			}
			else if(tokenList[i].getName() == "TEXT")
			{
				tokenList[i].setType("DIRETIVA");
			} 
			else if(tokenList[i].getName() == "DATA")
			{
				tokenList[i].setType("DIRETIVA");
			} 
			else if(tokenList[i].getName() == "SPACE")
			{
				tokenList[i].setType("DIRETIVA");
			} 
			else if(tokenList[i].getName() == "CONST")
			{
				tokenList[i].setType("DIRETIVA");
			} 
		}
	}
}

void Parser::isLabel(vector<Symbol>& labelTable, vector<Token>& tokenList)
{
	unsigned int i,j;
	string convert;

	for(i = 0; i < tokenList.size(); i++)
	{
		if(tokenList[i].getType().empty())
		{
			for(j = 0; j < labelTable.size(); j++)
			{
				if(tokenList[i].getName() == labelTable[j].getName())
				{
					tokenList[i].setType("LABEL");
					convert = to_string(labelTable[j].getAdress());
					tokenList[i].setOp(convert);
					break;
				}
			}
		}
	}
}