#ifndef _PREPROCESSING_HPP_
#define _PREPROCESSING_HPP_

#include <string>
#include <vector>
#include <boost/regex.hpp>

class Preprocessing
{
public:
	Preprocessing(const std::string& inputFile, const std::string& outputFile);
	virtual ~Preprocessing();
	void RemoveComments();
	void RemoveSpaces();
	void RemoveBlankLines();

private:
	std::string inputFile;
	std::string outputFile;
	std::vector<std::string> lineVector;
	static std::string regex_comment_str;
	static boost::regex regex_comment;
	static boost::regex regex_space;
	
};










#endif //_PREPROCESSING_HPP_