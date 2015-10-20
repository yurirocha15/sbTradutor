#include <parser.hpp>
#include <verboseLog.hpp>
#include <macro.hpp>
#include <boost/algorithm/string.hpp>


#include <iostream>
#include <cstdlib>

using namespace std;

Parser::Parser(const std::string& inputFile, const std::string& outputFile, std::vector<Symbol>& labelTable, std::vector<Token>& tokenList)
{
	//Variáveis para debug
	unsigned int i = 0;
	string testeName;
	string testeTipo;
	//-----------
	vector<string> lineVector;
	Macro scanner(inputFile,outputFile);
	lineVector = scanner.GetLineVector();
	tokenList = firstPass(lineVector, labelTable);
	this->secondPass(labelTable,tokenList);
	for(i = 0; i < tokenList.size(); i++)
	{
		testeName = tokenList[i].getName();
		testeTipo = tokenList[i].getType();
		log<LOG_DEBUG>("Nome: %1% Tipo: %2%") % testeName % testeTipo;

	}
	detectError(tokenList);
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
	unsigned int i;
	//A segunda Passagem irá classificar todos os elementos em Line Vector, simbolo, INSTRUÇÃO e diretiva;
	this->isInstruction(tokenList);
	this->isDirective(tokenList);
	this->isLabel(labelTable,tokenList);
	//Laço para detectar se algum token não foi classificado;
	for(i = 0; i < tokenList.size();i++)
	{
		size_t found = tokenList[i].getName().find_first_of("123456789");
		if(tokenList[i].getType().empty()&&(found != string::npos))
		{

		}
	}
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

void Parser::detectError(vector<Token> tokenList)
{
	unsigned int i;
	int linhaData = 0;
	int linhaText = 0;
	int dataAntes;
	int atual;
	int p1,p2,p3;
	//---------------------------------------Loop Que detecta a linha da Section Data e Text---------------------------
	for(i = 0; i < tokenList.size(); i++)
	{
		//Data vir depois ta section Text:
		if(tokenList[i].getName() == "DATA")
		{
			linhaData = tokenList[i].getLine();
		}
		else if(tokenList[i].getName() == "TEXT")
		{
			linhaData = tokenList[i].getLine();
		}
	}
	//---------------------Section Data ou Section Text faltante-------------------------
	if((linhaData|linhaText) == 0)
	{
		log<LOG_ERROR>("Linha 0: Section Data ou Text faltando", "Semântico");	
	}
	//-----------------------------------------------------------------------------------
	//-------------------------------Section Data em Antes da Section TEXT-------------------------------------------
	if(linhaData < linhaText)
	{
		dataAntes = 1;
		log<LOG_ERROR>("Linha %1%: Section Data ANTES da Section Text", "Semântico") % linhaData;
	}
	//---------------------------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------------------
	for(i = 0; i < tokenList.size(); i++)
	{
		//------------------------Diretivas ou Instruções na sessão errada Errada----------------------------------------
		if(!dataAntes)
		{
			if((tokenList[i].getName() == "CONST")||(tokenList[i].getName() == "SPACE"))
			{
				//Se a linha de const ou space for anterior a linha de Section Data
				if(tokenList[i].getLine() < linhaData)
				{
					log<LOG_ERROR>("Linha %1%: Diretiva ou Instrução na sessão errada", "Semântico") % tokenList[i].getLine();			
				}
			}
			if((tokenList[i].getType() == "INSTRUÇÃO") && (tokenList[i].getLine() < linhaData))
			{
				log<LOG_ERROR>("Linha %1%: Diretiva ou Instrução na sessão errada", "Semântico") % tokenList[i].getLine();
			}
		}
		else
		{
			if((tokenList[i].getName() == "CONST")|(tokenList[i].getName() == "SPACE"))
			{
				//Se a linha de const ou space for anterior a linha de Section Data
				if(tokenList[i].getLine() < linhaData)
				{
					log<LOG_ERROR>("Linha %1%: Diretiva ou Instrução na sessão errada", "Semântico") % tokenList[i].getLine();			
				}
			}
			if((tokenList[i].getType() == "INSTRUÇÃO")&&(tokenList[i].getLine() > linhaData))
			{
				log<LOG_ERROR>("Linha %1%: Diretiva ou Instrução na sessão errada", "Semântico") % tokenList[i].getLine();
			}
		} 
		//-----------------------------------------------------------------------------------------------------------
		//-------------------------------Instruções com quantidade de operando inválidas-----------------------------
		if(tokenList[i].getType() == "INSTRUÇÃO")
		{
			if(tokenList[i].getName() == "COPY")
			{
				atual = tokenList[i].getLine();
				p1 = tokenList[i+1].getLine();
				p2 = tokenList[i+2].getLine();
				p3 = tokenList[i+3].getLine();
				if((p1 != atual)||(p2 != atual) || (p3 == atual))
				{
					log<LOG_ERROR>("Linha %1%: Instrução com quantidade de operandos Inválidos", "Sintático") % tokenList[i].getLine();		
				}
			}
			else if(tokenList[i].getName() == "STOP")
			{
				atual = tokenList[i].getLine();
				p1 = tokenList[i+1].getLine();
				if((p1 == atual))
				{
					log<LOG_ERROR>("Linha %1%: Instrução com quantidade de operandos Inválidos", "Sintático") % tokenList[i].getLine();		
				}
			}
			else
			{
				atual = tokenList[i].getLine();
				p1 = tokenList[i+1].getLine();
				p2 = tokenList[i+2].getLine();
				if((p1 != atual)|(p2 == atual))
				{
					log<LOG_ERROR>("Linha %1%: Instrução com quantidade de operandos Inválidos", "Sintático") % tokenList[i].getLine();		
				}
			}
		}
		//------------------------------------------------------------------------------------------------------------
		//------------------------------------------Seção inválida----------------------------------------------------
		if((tokenList[i].getName() == "SECTION") && ((tokenList[i+1].getName() != "TEXT") && (tokenList[i+1].getName() != "DATA")))
		{
			log<LOG_ERROR>("Linha %1%: Seção Inválida", "Semântico") % tokenList[i].getLine();	
		}
		//---------------------------------------------------------------------------------------------------------------
		//-----------------------------------Tipo de Argumento Inválido--------------------------------------------------
	}
}