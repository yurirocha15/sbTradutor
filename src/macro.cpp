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
	this->inputFile = outputFile + ".pre";
	this->outputFile = outputFile + ".mcr";

	Lexical lexical;

	log<LOG_INFO>("Arquivos de Macro. Input: %1%. Output: %2%.") % this->inputFile % this->outputFile;

	try
	{
		Preprocessing preprocessing(inputFile, outputFile);
		lineVector = preprocessing.GetLineVector();
	}
	catch(const char* e)
	{
		throw e;
	}
	
	for(auto& line : lineVector)
	{
		log<LOG_DEBUG>("%1%") % line;
	}

	int sectionTextLine = INF;
	int sectionDataLine = INF;
	auto line = lineVector.begin();
	bool macroStarted = false;
	bool codeStarted = false;
	int index = 0;
	std::vector<std::string> *arguments = NULL;
	std::vector<std::string> tmp_defTable;
	for(int i = 0; i < lineVector.size(); ++line, i++)
	{
		MNT tmp_nameTable;
		if(*line == "SECTION TEXT")
		{
			sectionTextLine = i;
		}
		if(*line == "SECTION DATA")
		{
			sectionDataLine = i;
		}
		if (*line == "ENDMACRO")
		{
			if(!macroStarted)
			{
				log<LOG_ERROR>("Linha %1%: ENDMACRO sem inicialização de Macro.", "Semântico") % (i + 1);
			}
			else
			{
				defTable.push_back(tmp_defTable);
				macroStarted = false;
				delete arguments;
				arguments = NULL;
			}
		}
		else if(line->find(" MACRO ") != std::string::npos)
		{
			if(codeStarted)
			{
				log<LOG_ERROR>("Linha %1%: Macro inicializada após o inicio do código principal.", "Semântico") % (i + 1);
			}
			if(macroStarted)
			{
				log<LOG_ERROR>("Linha %1%: Macro iniciado dentro de outro macro. Ausência de ENDMACRO.", "Semântico") % (i + 1);
			}
			else
			{
				macroStarted = true;
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
					}
					else
					{
						words[0].pop_back();
						//Check if label is valid
						lexical.CheckLabel(words[0], i+1);
						tmp_nameTable.name = words[0];
						tmp_nameTable.numArg = words.size() - 2;
						tmp_nameTable.index = index++;
						nameTable.push_back(tmp_nameTable);
						//copy all the elements from the vector starting from the third one
						std::vector<std::string>::const_iterator first = std::begin(words) + 2;
						std::vector<std::string>::const_iterator last = std::end(words);
						arguments = new std::vector<std::string>(first, last);
					}
				}

			}
		}
		else if (macroStarted)
		{
			if(arguments != NULL)
			{
				for(auto& arg : *arguments)
				{
					log<LOG_DEBUG>("Arg: %1%") % arg;
					size_t pos;
					if((pos = line->find(" " + arg)) != std::string::npos)
					{
						line->replace(pos + 1, std::string(arg).length(), "#" + std::to_string((&arg - &(*arguments)[0]) + 1));
						log<LOG_DEBUG>("Macro:   %1%") % *line;
					}
				}
			}
			tmp_defTable.push_back(*line);
		}
		else
		{
			codeStarted = true;
		}
		
		if(codeStarted)
		{
			for(auto& nt : nameTable)
			{
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
						std::vector<std::string>::const_iterator first = std::begin(words) + 1;
						std::vector<std::string>::const_iterator last = std::end(words);
						arguments = new std::vector<std::string>(first, last);
						tmp_defTable = defTable[nt.index];
						
					}
				}
			}
		}
	}

	for(auto& tmp : defTable)
	{
		for(auto& tmp2 : tmp)
		{
			log<LOG_DEBUG>("DEFTABLE: %1%") % tmp2;
		}
	}
}

Macro::~Macro()
{
}

std::vector<std::string> Macro::GetLineVector()
{
	return lineVector;
}