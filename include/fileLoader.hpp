#ifndef _FILELOADER_HPP_
#define _FILELOADER_HPP_

#include <vector>
#include <string>

#include <token.hpp>

class FileLoader
{
public:
	FileLoader();
	virtual ~FileLoader();
	std::vector<std::string> LoadFile(const std::string& fileName);
	void SaveFile(const std::string& fileName, const std::vector<std::string>& lineVector);
	void SaveObjectFile(const std::string& fileName, const std::vector<Token>& tokenList);



};

#endif //_FILELOADER_HPP_