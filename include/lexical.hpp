#ifndef _LEXICAL_HPP_
#define _LEXICAL_HPP_

#include <string>
#include <boost/regex.hpp>

class Lexical
{
public:
	Lexical();
	virtual ~Lexical();
	bool CheckLabel(const std::string& label, const int& line);

private:
	static std::string regex_label_str;
	static boost::regex regex_label;
	
};










#endif //_LEXICAL_HPP_