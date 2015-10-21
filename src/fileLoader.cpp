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

	return lineVector;
}

void FileLoader::SaveFile(const string& fileName, const vector<string>& lineVector)
{
	ofstream outputFile (fileName);
	if(outputFile.is_open())
	{
		for(auto& line : lineVector)
		{
			outputFile << line << std::endl;
		}
	}
	else
	{
		log<LOG_ERROR>("Impossível criar o arquivo %1%.") % fileName;
		throw "Impossível criar o arquivo.";
	}
}

void FileLoader::SaveObjectFile(const std::string& fileName, const std::vector<Token>& tokenList)
{
	std::string outputString = "";
	ofstream outputFile (fileName);
	if(outputFile.is_open())
	{
		for(auto token : tokenList)
		{
			if(token.getType() != "DIRETIVA")
			{
				if(token.getType() == "INSTRUÇÃO")
				{
					outputString = outputString + token.getOp() + " ";
				}
				else if(token.getType() == "LABEL")
				{
					if(token.getName().back() != ':')
					{
						outputString = outputString + token.getOp() + " ";
					}
					else
					{
						if(token.getSpace_const() == "SPACE")
						{
							for(int i = 0; i < token.getSize(); i++)
							{
								outputString = outputString + "0 ";
							}
						}
						else if(token.getSpace_const() == "CONST")
						{
							outputString = outputString + std::to_string(token.getSize()) + " ";
						}
					}
				}
			}
		}
		outputString.pop_back();
		outputFile << outputString;
	}
	else
	{
		log<LOG_ERROR>("Impossível criar o arquivo %1%.") % fileName;
		throw "Impossível criar o arquivo.";
	}
}