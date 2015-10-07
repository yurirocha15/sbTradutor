#include <preprocessing.hpp>
#include <verboseLog.hpp>

Preprocessing::Preprocessing(std::string inputFile, std::string outputFile)
{
	this->inputFile = inputFile + ".asm";
	this->outputFile = outputFile + ".pre";

	log<LOG_INFO>("Arquivos de Preprocessamento. Input: %1%. Output: %2%.") % this->inputFile % this->outputFile;
}

Preprocessing::~Preprocessing()
{

}