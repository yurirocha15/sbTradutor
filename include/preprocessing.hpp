#ifndef _PREPROCESSING_HPP_
#define _PREPROCESSING_HPP_

#include <string>
#include <vector>
#include <boost/regex.hpp>

typedef struct Equ
{
	int value;
	std::string label;
	Equ(int value, std::string label)
	{
		this->value = value;
		this->label = label;
	}
}Equ;

class Preprocessing
{
public:
	Preprocessing(const std::string& inputFile, const std::string& outputFile);
	virtual ~Preprocessing();
	void Process();
	std::vector<std::string> GetLineVector();

private:
	std::string inputFile;
	std::string outputFile;
	std::vector<std::string> lineVector;
	static boost::regex regex_comment;
	static boost::regex regex_space;
	int sectionTextLine;
	int sectionDataLine;
	
};










#endif //_PREPROCESSING_HPP_