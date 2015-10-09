#include <preprocessing.hpp>
#include <verboseLog.hpp>
#include <fileLoader.hpp>
#include <boost/algorithm/string/trim.hpp>

std::string Preprocessing::regex_comment_str = ";.*$";
boost::regex Preprocessing::regex_comment (regex_comment_str);
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
	RemoveComments();
	RemoveSpaces();
	RemoveBlankLines();

	for(auto& line : lineVector)
	{
		std::cout << line << std::endl;
	}

}

Preprocessing::~Preprocessing()
{

}

void Preprocessing::RemoveComments()
{
	boost::smatch matches;
	for(std::string& line : lineVector)
	{
		if(boost::regex_search(line, matches, regex_comment))
		{
			line = boost::regex_replace(line, regex_comment, "");
		}
	}
}

void Preprocessing::RemoveSpaces()
{
	boost::smatch matches;
	for(std::string& line : lineVector)
	{
		boost::trim(line);
		if(boost::regex_search(line, matches, regex_space))
		{
			line = boost::regex_replace(line, regex_space, " ");
		}
	}
}

void Preprocessing::RemoveBlankLines()
{
	lineVector.erase(std::remove_if(lineVector.begin(), lineVector.end(), [](std::string line){return line == "";}), lineVector.end());
}