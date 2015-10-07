#ifndef _PREPROCESSING_HPP_
#define _PREPROCESSING_HPP_

#include <string>

class Preprocessing
{
public:
	Preprocessing(std::string inputFile, std::string outputFile);
	virtual ~Preprocessing();

private:
	std::string inputFile;
	std::string outputFile;
	
};










#endif //_PREPROCESSING_HPP_