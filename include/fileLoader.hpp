#ifndef _FILELOADER_HPP_
#define _FILELOADER_HPP_

#include <vector>
#include <string>

class FileLoader
{
public:
	FileLoader();
	virtual ~FileLoader();
	std::vector<std::string> LoadFile(std::string fileName);
	void SaveFile(std::string fileName, std::vector<std::string> lineVector);



};

#endif //_FILELOADER_HPP_