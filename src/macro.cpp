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
	}
	catch(const char* e)
	{
		throw e;
	}

	FileLoader fileLoader;
	try
	{
		lineVector = fileLoader.LoadFile(this->inputFile);
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
	int index = 0;
	std::vector<std::string> *arguments;
	for(int i = 0; line != lineVector.end(); ++line, i++)
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
		if(line->find(" MACRO ") != std::string::npos)
		{
			if(macroStarted)
			{
				log<LOG_ERROR>("Linha %1%: Macro iniciado dentro de outro macro.", "Semântico") % (i + 1);
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
						//arguments = new std::vector<std::string>(advance(begin(words), 2), end(words));
					}
				}

			}
		}
	}
}

Macro::~Macro()
{
}