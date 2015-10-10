#include <preprocessing.hpp>
#include <verboseLog.hpp>
#include <fileLoader.hpp>
#include <lexical.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>

#define INF 2147483647

boost::regex Preprocessing::regex_comment (";.*$");
boost::regex Preprocessing::regex_space ("[[:space:]]+", boost::regex_constants::egrep);


Preprocessing::Preprocessing(const std::string& inputFile, const std::string& outputFile)
{
	this->inputFile = inputFile + ".asm";
	this->outputFile = outputFile + ".pre";

	log<LOG_INFO>("Arquivos de Preprocessamento. Input: %1%. Output: %2%.") % this->inputFile % this->outputFile;

	FileLoader fileLoader;
	try
	{
		lineVector = fileLoader.LoadFile(this->inputFile);
	}
	catch(const char* e)
	{
		throw e;
	}

	//Salva as linhas que iniciam as seções de data e texto como "infinito" para sabermos se as diretivas EQU estão nas posições certas.
	sectionTextLine = INF;
	sectionDataLine = INF;
	RemoveCommentsAndSpacesAndBlankLines();


	for(auto& line : lineVector)
	{
		std::cout << line << std::endl;
	}

}

Preprocessing::~Preprocessing()
{

}

void Preprocessing::RemoveCommentsAndSpacesAndBlankLines()
{
	Lexical lexical;
	boost::smatch matches;
	std::vector<Equ> equs;
	auto line = lineVector.begin();
	for(int i = 0; line != lineVector.end(); ++line, i++)
	{
		if(boost::regex_search(*line, matches, regex_comment))
		{
			*line = boost::regex_replace(*line, regex_comment, "");
		}
		boost::trim(*line);
		if(boost::regex_search(*line, matches, regex_space))
		{
			*line = boost::regex_replace(*line, regex_space, " ");
		}
		if(*line == "SECTION TEXT")
		{
			sectionTextLine = i;
		}
		if(*line == "SECTION DATA")
		{
			sectionDataLine = i;
		}
		if(line->find(" EQU ") != std::string::npos)
		{
			if(i > sectionTextLine)
				log<LOG_ERROR>("Linha %1%: EQU depois da SECTION Text.", "Semântico") % (i+1);

			std::vector<std::string> words;
			boost::split(words, *line, boost::is_any_of("\t "));
			if(words[1] != "EQU")
			{
				log<LOG_ERROR>("Linha %1%: EQU inválido.", "Sintático") % (i+1);
			}
			else
			{
				Equ tmp_equ(std::stoi(words[2]), words[0]);
				equs.push_back(tmp_equ);
				log<LOG_DEBUG>("%1% equals %2%.") % tmp_equ.label % tmp_equ.value;
				lexical.CheckLabel(tmp_equ.label, i+1);
				*line = "";
			}
		}
		if(line->find("IF ") != std::string::npos)
		{
			if(i < sectionTextLine)
				log<LOG_ERROR>("Linha %1%: IF antes da SECTION Text.", "Semântico") % (i+1);

			std::vector<std::string> words;
			boost::split(words, *line, boost::is_any_of("\t "));
			if(words[0] != "IF")
			{
				log<LOG_ERROR>("Linha %1%: IF inválido.", "Sintático") % (i+1);
			}
			else
			{
				for(auto& tmp_equ : equs)
				{
					if(words[1] == tmp_equ.label)
					{
						if(tmp_equ.value)
						{
							*line = "";
						}
						else
						{
							*line = "";
							++line;
							*line = "";

						}
					}
				}
			}

		}



	}
	lineVector.erase(std::remove_if(lineVector.begin(), lineVector.end(), [](std::string line){return line == "";}), lineVector.end());
}
