#ifndef _PREPROCESSING_HPP_
#define _PREPROCESSING_HPP_

#include <string>
#include <vector>

class Preprocessing
{
public:
	Preprocessing(std::string inputFile, std::string outputFile);
	virtual ~Preprocessing();

private:
	std::string inputFile;
	std::string outputFile;
	std::vector<std::string> lineVector;
	
};










#endif //_PREPROCESSING_HPP_