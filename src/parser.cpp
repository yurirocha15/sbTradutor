#include <parser.hpp>
#include <verboseLog.hpp>
#include <macro.hpp>
#include <fileLoader.hpp>
#include <boost/algorithm/string.hpp>


#include <iostream>
#include <cstdlib>

using namespace std;

Parser::Parser(const std::string& inputFile, const std::string& outputFile, std::vector<Symbol>& labelTable, std::vector<Token>& tokenList) : hasError(false)
{
	this->outputFile = outputFile + ".o";
	this->inputFile = inputFile + ".mcr";

	log<LOG_INFO>("Arquivos de Macro. Input: %1%. Output: %2%.") % this->inputFile % this->outputFile;

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
		log<LOG_DEBUG>("Nome: %1% Tipo: %2% Linha: %3%") % testeName % testeTipo %tokenList[i].getOp();
		if(tokenList[i].getSize() != 999)
		{
			log<LOG_DEBUG>("Tamanho Memoria: %1% ") % tokenList[i].getSize();	
		}
		
	}
	detectError(labelTable, tokenList);

	//Caso nao tenha erros. Imprime o arquivo traduzido
	if(!hasError)
	{
		FileLoader file;
		file.SaveObjectFile(this->outputFile, tokenList);
	}
	else
	{
		log<LOG_WARNING>("Erros encontrados no programa. O arquivo de saída não será gerado.");
	}
}

Parser::~Parser()
{

}

std::vector<Token> Parser::firstPass(std::vector<std::string> lineVector, std::vector<Symbol>& labelTable)
{
	//método responsável em executar a primeira passagem, ou seja, irá percorrer todas as linhas, e quanto achar labels 
	//(tokens terminados em ":") adiciona-lo a tabela de simbolos, juntamente com seu endereço

	unsigned int i,j,k; //contadores
	int adress = 0;
	int achouRotulo;
	vector<string> tokensLine;
	std::vector<Token> tokenList;
	char last;
	string labelName;

	for(i = 0; i < lineVector.size(); i++)
	{
		achouRotulo = 0;
		boost::split(tokensLine,lineVector[i],boost::is_any_of(" "));
		for(j = 0;j < tokensLine.size();j++)
		{
			last = tokensLine[j].back();
			if(last == ':' )
			{
				if(achouRotulo)
				{
					log<LOG_ERROR>("Linha %1%: Dois Rótulos na mesma Linha", "Semântico") % (i+1); 	
					hasError = true;
				}
				achouRotulo = 1;
				labelName = tokensLine[j].substr(0,tokensLine[j].size()-1);
				for(k = 0; k < labelTable.size();k++)
				{
					if(labelName == labelTable[k].getName())
					{
						log<LOG_ERROR>("Linha %1%: Declaração Repetida", "Semântico") % (i+1); 
						hasError = true;
					}
				}
				Token token(tokensLine[j],i+1);

				token.setType("LABEL");
				tokenList.push_back(token);
				
				Symbol label(labelName,i+1,adress);
				labelTable.push_back(label);
			}
			else if(tokensLine[j] == "CONST")
			{
				if(j+1 < tokensLine.size())
				{
					size_t found = tokensLine[j+1].find_first_of("X0123456789");
					if(found != string::npos)
					{
						size_t found = tokensLine[j+1].find_first_of("X");
						if(found != string::npos)
						{
							int decimal = 0;
							try
							{
								decimal = stoi(tokensLine[j+1],0,16);
							}
							catch(const invalid_argument& e)
							{
								log<LOG_ERROR>("Linha %1%: Hexadecimal inválido") % (i+1);
								hasError = true;
							}
							tokenList.back().setSize(decimal);
							tokenList.back().setSpace_const("CONST");
							labelTable.back().setSize(decimal);
							labelTable.back().setSpace_const("CONST");
							adress ++;
							break;
						}
						tokenList.back().setSize(stoi(tokensLine[j+1]));
						tokenList.back().setSpace_const("CONST");
						labelTable.back().setSize(stoi(tokensLine[j+1]));
						labelTable.back().setSpace_const("CONST");
						adress ++;
						break;
					}
				}
				else
				{
					log<LOG_ERROR>("Linha %1%: constante não definido", "Sintático") % (i+1); 
					hasError = true;
				}
			}
			else if(tokensLine[j] == "SPACE")
			{
				if(j+1 < tokensLine.size())
				{
					size_t found = tokenList[i].getName().find_first_of("0123456789");
					if(found)
					{
						tokenList.back().setSize(stoi(tokensLine[j+1]));
						tokenList.back().setSpace_const("SPACE");
						labelTable.back().setSize(stoi(tokensLine[j+1]));
						labelTable.back().setSpace_const("SPACE");
						adress += stoi(tokensLine[j+1]);
						break;
					}
					else
					{
						tokenList.back().setSpace_const("SPACE");
						tokenList.back().setSize(1);
						labelTable.back().setSpace_const("SPACE");
						labelTable.back().setSize(1);
						adress++;	
					}
				}
				else
				{
					tokenList.back().setSpace_const("SPACE");
					tokenList.back().setSize(1);
					labelTable.back().setSpace_const("SPACE");
					labelTable.back().setSize(1);
					adress++;	
				}
			}
			else
			{
				Token token(tokensLine[j],i+1);
				tokenList.push_back(token);
				if((tokensLine[j] != "SECTION")&&(tokensLine[j] != "DATA")&&(tokensLine[j] != "TEXT"))
				{
					adress++;	
				}
				
			}
		}
		tokensLine.erase(tokensLine.begin(),tokensLine.end());
	}
	return tokenList;
}

void Parser::secondPass(std::vector<Symbol>& labelTable, std::vector<Token>& tokenList)
{
	string last;
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
	string convert2;
	vector<string> split_string;
	size_t found;
	for(i = 0; i < tokenList.size(); i++)
	{
		if(tokenList[i].getType().empty())
		{
			found = tokenList[i].getName().find("+");
			if(found == string::npos)
			{
				for(j = 0; j < labelTable.size(); j++)
				{
					std::string tokenName = tokenList[i].getName();
					if(tokenName.back() == ',')
					{
						tokenName.pop_back();
					}
					if(tokenName == labelTable[j].getName())
					{
						tokenList[i].setType("LABEL");
						convert = to_string(labelTable[j].getAdress());
						tokenList[i].setOp(convert);
						tokenList[i].setSize(labelTable[j].getSize());
						tokenList[i].setSpace_const(labelTable[j].getSpace_const());
						break;
					}
				}
			}
			else
			{
				string temp = tokenList[i].getName();
				boost::split(split_string,temp,boost::is_any_of("+"));
				for(j = 0; j < labelTable.size(); j++)
				{
					std::string tokenName = split_string[0];
					if(tokenName.back() == ',')
					{
						tokenName.pop_back();
					}
					if(tokenName == labelTable[j].getName())
					{
						tokenList[i].setType("LABEL");
						int soma = 0;
						if(split_string.size() == 2)
						{
							soma = stoi(split_string[1]);
							convert = to_string((labelTable[j].getAdress()+stoi(split_string[1])));
						}
						else
						{
							log<LOG_ERROR>("Linha %1%: Estrutra de Acesso de memória incorreto ", "Sintático") % tokenList[i].getLine();
							convert = to_string(labelTable[j].getAdress());
						}
						tokenList[i].setOp(convert);
						tokenList[i].setSize(labelTable[j].getSize());
						tokenList[i].setSpace_const(labelTable[j].getSpace_const());
						if(soma >= tokenList[i].getSize())
						{
							log<LOG_ERROR>("Linha %1%: Acesso de memória não reservado ", "Semântico") % tokenList[i].getLine();
						}
						break;
					}
				}	
			}
		}
	}
}

void Parser::detectError(vector<Symbol>& labelTable, vector<Token> tokenList)
{
	unsigned int i,j;
	int linhaData = 0;
	int linhaText = 0;
	int existeLabel;
	int dataAntes;
	int atual;
	int p1,p2,p3;
	int labelDeclaration = 0;
	bool hasStop = false;
	string last;
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
			linhaText = tokenList[i].getLine();
		}
	}
	log<LOG_DEBUG>("Linha data %1% Linha text %2%") % linhaData % linhaText;
	//---------------------Section Data ou Section Text faltante-------------------------
	if((linhaData||linhaText) == 0)
	{
		log<LOG_ERROR>("Linha 0: Section Data ou Text faltando", "Semântico");	
		hasError = true;
	}
	//-----------------------------------------------------------------------------------
	//-------------------------------Section Data em Antes da Section TEXT-------------------------------------------
	if(linhaData < linhaText)
	{
		dataAntes = 1;
		log<LOG_ERROR>("Linha %1%: Section Data ANTES da Section Text", "Semântico") % linhaData;
		hasError = true;
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
					log<LOG_ERROR>("Linha %1%: 1Diretiva ou Instrução na sessão errada", "Semântico") % tokenList[i].getLine();			
					hasError = true;
				}
			}
			if((tokenList[i].getType() == "INSTRUÇÃO") && (tokenList[i].getLine() < linhaData))
			{
				log<LOG_ERROR>("Linha %1%: 2Diretiva ou Instrução na sessão errada", "Semântico") % tokenList[i].getLine();
				hasError = true;
			}
		}
		else
		{
			if((tokenList[i].getName() == "CONST")||(tokenList[i].getName() == "SPACE"))
			{
				//Se a linha de const ou space for anterior a linha de Section Data
				if(tokenList[i].getLine() < linhaData)
				{
					log<LOG_ERROR>("Linha %1%: 3Diretiva ou Instrução na sessão errada", "Semântico") % tokenList[i].getLine();			
					hasError = true;
				}
			}
			if((tokenList[i].getType() == "INSTRUÇÃO")&&(tokenList[i].getLine() > linhaData))
			{
				log<LOG_ERROR>("Linha %1%: 4Diretiva ou Instrução na sessão errada", "Semântico") % tokenList[i].getLine();
				hasError = true;
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
					hasError = true;
				}
				//-----------------------------------Tipo de Argumento Inválido--------------------------------------------------
				if(((p1 == atual)&&(tokenList[i+1].getType() != "LABEL"))||((p2 == atual)&&(tokenList[i+2].getType() != "LABEL")))
				{
					log<LOG_ERROR>("Linha %1%: Tipo de Argumento Inválido", "Sintático") % tokenList[i].getLine();
					hasError = true;
				}
				//----------------------------------------------------------------------------------------------------------------
				//----------------------------------------------Copy sem vírgula--------------------------------------------------
				if(tokenList[i+1].getName().back() != ',')
				{
					log<LOG_ERROR>("Linha %1%: Estrutura inválida da instrução COPY.", "Sintático") % tokenList[i].getLine();
					hasError = true;
				}
			}
			else if(tokenList[i].getName() == "STOP")
			{
				hasStop = true;
				atual = tokenList[i].getLine();
				p1 = tokenList[i+1].getLine();
				if((p1 == atual))
				{
					log<LOG_ERROR>("Linha %1%: Instrução com quantidade de operandos Inválidos", "Sintático") % tokenList[i].getLine();		
					hasError = true;
				}
			}
			else
			{
				atual = tokenList[i].getLine();
				p1 = tokenList[i+1].getLine();
				p2 = tokenList[i+2].getLine();
				if((p1 != atual)||(p2 == atual))
				{
					log<LOG_ERROR>("Linha %1%: Instrução com quantidade de operandos Inválidos", "Sintático") % tokenList[i].getLine();		
					hasError = true;
				}
				//-----------------------------------Tipo de Argumento Inválido--------------------------------------------------
				if(((p1 == atual)&&(tokenList[i+1].getType() != "LABEL")))
				{
					log<LOG_ERROR>("Linha %1%: Tipo de Argumento Inválido", "Sintático") % tokenList[i].getLine();
					hasError = true;
				}
				//----------------------------------------------------------------------------------------------------------------
			}
		}
		//For do yuri para divisao 0;
			/*for(j = 0; j < labelTable.size(); j++)
			{
				if(tokenList[i+1].getName() == labelTable[j].getName())
				{
					tokenList[i].setType("LABEL");
					convert = to_string(labelTable[j].getAdress());
					tokenList[i].setOp(convert);
					break;
				}
			}*/
		//------------------------------------------------------------------------------------------------------------
		//------------------------------------------Seção inválida----------------------------------------------------
		if((tokenList[i].getName() == "SECTION") && ((tokenList[i+1].getName() != "TEXT") && (tokenList[i+1].getName() != "DATA")))
		{
			log<LOG_ERROR>("Linha %1%: Seção Inválida", "Sintático") % tokenList[i].getLine();	
			hasError = true;
		}
		//---------------------------------------------------------------------------------------------------------------
		//----------------------------------Declaração Ausente-----------------------------------------------------------
		if(i !=0)
		{
			if((tokenList[i-1].getType() == "INSTRUÇÃO")&&(tokenList[i].getType().empty()))
			{
				log<LOG_ERROR>("Linha %1%: Declaração Ausente", "Semântico") % tokenList[i].getLine();
				hasError = true;
				cout<<"Declaração Ausente, Linha: "<<tokenList[i].getLine()<<endl;
			}
		}
		//----------------------------------------------------------------------------------------------------------
		//-----------------------------------Pulos para rótulos Inválidos---------------------------------------------
		if((tokenList[i].getName() == "JMP")||(tokenList[i].getName() == "JMPP")||(tokenList[i].getName() == "JMPN")||(tokenList[i].getName() == "JMPZ"))
		{
			existeLabel = 0;
			for(j = 0; j < labelTable.size(); j++)
			{
				if(tokenList[i+1].getName() == labelTable[j].getName())
				{
					existeLabel = 1;
				}
			}
			if(existeLabel == 0)
			{
				log<LOG_ERROR>("Linha %1%: Pulo para rótulo Inválido", "Semântico") % tokenList[i].getLine();
				hasError = true;
				//cout<<"Pulo para rótulo inválido, Linha: "<<tokenList[i].getLine()<<endl;
			}
			
		}
	}
	//Código sem instrução stop
	if(!hasStop)
	{
		log<LOG_ERROR>("Linha 0: Código sem instrução STOP.", "Semântico");	
		hasError = true;
	}
	//Laço para detectar se algum token não foi classificado;
	for(i = 0; i < tokenList.size();i++)
	{
		//Divisão por 0
		
		if(tokenList[i].getName() == "DIV")
		{
			if(tokenList[i+1].getSize() == 0)
			{
				log<LOG_ERROR>("Linha %1%: Divisão Por Zero", "Semântico") % tokenList[i].getLine();
				hasError = true;
			}
		}
		if(tokenList[i].getType().empty())
		{
			//Se ele for o primeiro elemento da linha ou depois de uma declaração de label na mesma linha:
			if(i != 0)
			{
				last = tokenList[i-1].getName().back();
				if(last == ":")
				{
					labelDeclaration = 1;
				}
				else
				{
					labelDeclaration = 0;
				}
				//---------------------------------------Instrução Inválida------------------------------------------
				if((tokenList[i-1].getLine() < tokenList[i].getLine())||labelDeclaration)
				{
					//Se a palavra Desconhecida estiver entre a  linha Text e DATA e passar na condição Acima
					if((tokenList[i].getLine() > linhaText)&&(tokenList[i].getLine() < linhaData))
					{
						cout<<"Instrução inválida!! Linha:"<<tokenList[i].getLine()<<endl;
						log<LOG_ERROR>("Linha %1%: Instrução Inválida", "Sintático") % tokenList[i].getLine();
						hasError = true;
					}
							
				}
				//---------------------------------------------------------------------------------------------------
			}
			if((tokenList[i].getLine() > linhaData)&&(labelDeclaration))
			{
				if((tokenList[i].getName() != "SPACE")&&(tokenList[i].getName() != "CONST"))
				{
					log<LOG_ERROR>("Linha %1%: Diretiva Inválida", "Sintático") % tokenList[i].getLine();
					hasError = true;
					cout<<"Diretiva inválida!! Linha:"<<tokenList[i].getLine()<<endl;
				}
			}	
		}
		//-----------------------------------Diretiva Inválida-----------------------------------------------
		//Se o token atual é DATA OU TEXT e o anterior não é SECTION:
		atual = i;
		if((atual-1) >= 0)
		{
			if((tokenList[i-1].getName() != "SECTION")&&((tokenList[i].getName() == "DATA")||(tokenList[i].getName() == "TEXT")))
			{
				log<LOG_ERROR>("Linha %1%: Diretiva Inválida", "Sintático") % tokenList[i].getLine();	
				hasError = true;
				//cout<<"Diretiva Inválida Linha: "<<tokenList[i].getLine()<<endl;
			}
		}
		if((tokenList[i].getName() == "STORE")&&(tokenList[i+1].getSpace_const() == "CONST"))
		{
			log<LOG_ERROR>("Linha %1%: Modificação de um valor Constante", "Semântico") % tokenList[i].getLine();
			hasError = true;
		}
	}
}