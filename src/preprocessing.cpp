#include <preprocessing.hpp>
#include <verboseLog.hpp>
#include <fileLoader.hpp>

Preprocessing::Preprocessing(std::string inputFile, std::string outputFile)
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
}

Preprocessing::~Preprocessing()
{

}