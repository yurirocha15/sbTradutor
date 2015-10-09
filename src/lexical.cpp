#include <lexical.hpp>
#include <verboseLog.hpp>

#include <iostream>

std::string Lexical::regex_label_str = "([_a-zA-Z][a-zA-Z0-9_]*)";
boost::regex Lexical::regex_label (regex_label_str);

Lexical::Lexical()
{

}

Lexical::~Lexical()
{

}

bool Lexical::CheckLabel(const std::string& label, const int& line)
{
	try
	{
		if (boost::regex_match (label, regex_label))
			return false;
		else
		{
			log<LOG_ERROR>("Linha %1%: A Variável ou Rotulo %2% é Invalido.", "Léxico") % line % label;
			return true;
		}
	}
	catch (boost::regex_error e)
	{
		std::cerr << "Error on label checking. " << e.what() << std::endl;
	}
	return false;
}