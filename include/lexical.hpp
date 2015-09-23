#ifndef _LEXICAL_HPP_
#define _LEXICAL_HPP_

#include <string>
#include <boost/regex.hpp>

class Lexical
{
public:
	Lexical();
	~Lexical();
	bool CheckLabel(std::string label);

private:
	static std::string regex_label_str;
	static boost::regex regex_label;
	
};










#endif //_LEXICAL_HPP_