#include <algorithm>
#include <boost/algorithm/string.hpp>



#include <macro.hpp>
#include <preprocessing.hpp>
#include <verboseLog.hpp>
#include <fileLoader.hpp>
#include <lexical.hpp>

#define INF 2147483647


Macro::Macro(const std::string& inputFile, const std::string& outputFile)
{
	//Pra checagem da label do macro
	Lexical lexical;

	this->outputFile = outputFile + ".mcr";
	this->inputFile = inputFile + ".pre";

	log<LOG_INFO>("Arquivos de Macro. Input: %1%. Output: %2%.") % this->inputFile % this->outputFile;

	//Inicia o preprocessamento antes de fazer o processamento de macro
	try
	{
		Preprocessing preprocessing(inputFile, outputFile);
		lineVector = preprocessing.GetLineVector();
	}
	catch(const char* e)
	{
		throw e;
	}

	//Salva a posicao das sections test e data para checagem de erros
	int sectionTextLine = INF;
	int sectionDataLine = INF;
	//Utilizados para a checagem de erros
	bool macroStarted = false;
	bool codeStarted = false;
	//index para salvar a posição no macro na MNT
	int index = 0;
	//Salva os argumentos de entrada do macro temporariamente
	std::vector<std::string> *arguments = NULL;
	//Salva as informacoes da MDT temporariamenta
	std::vector<std::string> tmp_defTable;
	//Iterador para percorrer o vetor de linhas
	auto line = lineVector.begin();

	for(int i = 0; i < (int)lineVector.size(); ++line, i++)
	{
		//MNT temporaria
		MNT tmp_nameTable;
		std::string::size_type tmp_pos;
		//Salva a linha do section text
		if(*line == "SECTION TEXT")
		{
			sectionTextLine = i;
		}
		//Salva a linha do section data
		else if(*line == "SECTION DATA")
		{
			sectionDataLine = i;
		}
		//Caso a linha seja igual a um endmacro
		else if (*line == "ENDMACRO")
		{
			//Checa o erro de endmacro sem iniciar um macro.
			if(!macroStarted)
			{
				log<LOG_ERROR>("Linha %1%: ENDMACRO sem inicialização de Macro.", "Semântico") % (i + 1);
			}
			else
			{
				//Se nao tem erro, salva a tabela de definicoes no vetor definitivo e restaura as variaveis temporarias para os valores iniciaris
				defTable.push_back(tmp_defTable);
				macroStarted = false;
				delete arguments;
				arguments = NULL;
				tmp_defTable.clear();
			}
			//Linhas vazias sao excluidas no fim da passagem
			*line = "";
		}
		//Caso a palavra macro exista na linha. 2 checagens sao necessarias para evitar que labels macro2 sejam classificadas como macros
		else if((tmp_pos = line->find(" MACRO ")) != std::string::npos || ((tmp_pos = line->find(" MACRO")) != std::string::npos && tmp_pos + 6 == line->size()))
		{
			//Checa se o codigo principal ja iniciou, se sim, lança um erro
			if(codeStarted)
			{
				log<LOG_ERROR>("Linha %1%: Macro inicializada após o inicio do código principal.", "Semântico") % (i + 1);
			}
			//Checa se um macro ja foi iniciado e ainda nao terminou
			if(macroStarted)
			{
				log<LOG_ERROR>("Linha %1%: Macro iniciado dentro de outro macro. Ausência de ENDMACRO.", "Semântico") % (i + 1);
			}
			else
			{
				//Sinaliza que o macro foi iniciado
				macroStarted = true;
				//Checa se a definição do macro esta dentro da seção texto
				if(i < sectionTextLine || i > sectionDataLine)
				{
					log<LOG_ERROR>("Linha %1%: Macro definido fora da seção de texto.") % (i+1);
				}
				std::vector<std::string> words;
				boost::split(words, *line, boost::is_any_of("\t "));

				if(words[1] != "MACRO")
				{
					log<LOG_ERROR>("Linha %1%: Macro inválido.", "Sintático") % (i+1);
				}
				else
				{
					if(words[0].back() != ':')
					{
						//TODO: Erro label sem :
						//Gera erro sintatico de operacao nao definida
					}
					else
					{
						words[0].pop_back();
						//Checa se a labe é valida
						lexical.CheckLabel(words[0], i+1);
						//Salva as informações na tabela de nomes
						tmp_nameTable.name = words[0];
						tmp_nameTable.numArg = words.size() - 2;
						tmp_nameTable.index = index++;
						//Salva no vetor da tabela de nomes
						nameTable.push_back(tmp_nameTable);
						//copy all the elements from the vector starting from the third one
						std::vector<std::string>::const_iterator first = std::begin(words) + 2;
						std::vector<std::string>::const_iterator last = std::end(words);
						arguments = new std::vector<std::string>(first, last);
					}
				}

			}
			//As linhas vazias sao removidas no fim da passagem
			*line = "";
		}
		//Caso um macro ja tenha sido iniciado
		else if (macroStarted)
		{
			//Checa se a macro tem argumentos
			if(arguments != NULL)
			{
				for(auto& arg : *arguments)
				{
					std::string::size_type pos;
					if((pos = line->find(" " + arg)) != std::string::npos)
					{
						line->replace(pos + 1, std::string(arg).length(), "#" + std::to_string((&arg - &(*arguments)[0]) + 1));
					}
				}
			}
			tmp_defTable.push_back(*line);
			*line = "";
		}
		else
		{
			codeStarted = true;
		}
		
		if(codeStarted)
		{
			for(auto& nt : nameTable)
			{
				bool firstLineDefined = false;
				if(line->find(nt.name) != std::string::npos)
				{
					std::vector<std::string> words;
					boost::split(words, *line, boost::is_any_of("\t "));
					if(words[0] == nt.name)
					{
						if(words.size() - 1 != nt.numArg)
						{
							log<LOG_ERROR>("Linha %1%: Numero incorreto de argumentos da macro %2%.", "Semântico") % (i + 1) % nt.name;
						}
						else
						{
							std::vector<std::string>::const_iterator first = std::begin(words) + 1;
							std::vector<std::string>::const_iterator last = std::end(words);
							arguments = new std::vector<std::string>(first, last);
							tmp_defTable = defTable[nt.index];

							std::vector<std::string> words2;
							boost::split(words2, tmp_defTable[0], boost::is_any_of("\t "));
							tmp_defTable[0] = "";
							for(auto& word : words2)
							{
								if(word[0] == '#')
								{
									word.erase(word.begin());
									word = (*arguments)[std::stoi(word) - 1]; 
								}
								tmp_defTable[0] += " " + word;
							}
							boost::trim(tmp_defTable[0]);	
							firstLineDefined = true;
						}				
					}
					else if(words[1] == nt.name)
					{
						if(words.size() - 2 != nt.numArg)
						{
							log<LOG_ERROR>("Linha %1%: Numero incorreto de argumentos da macro %2%.", "Semântico") % (i + 1) % nt.name;
						}
						else
						{
							std::vector<std::string>::const_iterator first = std::begin(words) + 2;
							std::vector<std::string>::const_iterator last = std::end(words);
							arguments = new std::vector<std::string>(first, last);
							tmp_defTable = defTable[nt.index];

							std::vector<std::string> words2;
							boost::split(words2, tmp_defTable[0], boost::is_any_of("\t "));
							tmp_defTable[0] = words[0];
							for(auto& word : words2)
							{
								if(word[0] == '#')
								{
									word.erase(word.begin());
									word = (*arguments)[std::stoi(word) - 1]; 
								}
								tmp_defTable[0] += " " + word;
							}
							boost::trim(tmp_defTable[0]);	
							firstLineDefined = true;
						}	
					}
					
					if(firstLineDefined)
					{
						*line = tmp_defTable[0];
						tmp_defTable.erase(std::begin(tmp_defTable));

						for(auto& line_defTable : tmp_defTable)
						{
							std::vector<std::string> words2;
							boost::split(words2, line_defTable, boost::is_any_of("\t "));
							line_defTable = "";
							for(auto& word : words2)
							{
								if(word[0] == '#')
								{
									word.erase(word.begin());
									word = (*arguments)[std::stoi(word) - 1]; 
								}
								line_defTable += " " + word;
							}
							boost::trim(line_defTable);

							if(line != std::end(lineVector))
							{
								++line;
								i++;
								line = lineVector.insert(line, line_defTable);
							}
							else
							{
								lineVector.push_back(line_defTable);
								i++;
								++line;
							}

							
						}
					}	
				}
			}
		}
	}
	lineVector.erase(std::remove_if(lineVector.begin(), lineVector.end(), [](std::string line){return line == "";}), lineVector.end());

	FileLoader file;
	file.SaveFile(this->outputFile, lineVector);
}

Macro::~Macro()
{
}

std::vector<std::string> Macro::GetLineVector()
{
	return lineVector;
}