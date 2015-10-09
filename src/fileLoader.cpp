#include <fileLoader.hpp>
#include <verboseLog.hpp>

#include <algorithm>
#include <fstream>

using namespace std;

FileLoader::FileLoader()
{

}

FileLoader::~FileLoader()
{

}

vector<string> FileLoader::LoadFile(const string& fileName)
{
	//Cria string e vetor de strings que irá salvar o texto de entrada
	string line;
	vector<string> lineVector;
	ifstream inputFile (fileName);

	if(inputFile.is_open())
	{
		while(getline(inputFile,line))
		{
			transform(line.begin(), line.end(), line.begin(), ::toupper);
			lineVector.push_back(line);
		}
		inputFile.close();
	}
	else
	{
		//caso o arquivo não exista ou não possa ser aberto, o método lançará um erro para avisar o programa que o chamou.
		log<LOG_ERROR>("Impossível abrir o arquivo %1%.") % fileName;
		throw "Impossível abrir o arquivo.";
	}
	for(std::string line : lineVector)
	{
		
	}

	return lineVector;
}

void FileLoader::SaveFile(const string& fileName, const vector<string>& lineVector)
{
	ofstream outputFile (fileName);
	if(outputFile.is_open())
	{
		for(unsigned int i = 0; i < lineVector.size(); i++)
		{
			outputFile << lineVector[i];
		}
	}
	else
	{
		log<LOG_ERROR>("Impossível criar o arquivo %1%.") % fileName;
		throw "Impossível criar o arquivo.";
	}
}