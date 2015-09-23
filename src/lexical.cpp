#include "lexical.hpp"
#include "verboseLog.hpp"

#include <iostream>

std::string Lexical::regex_label_str = "(^[a-zA-Z][a-zA-Z0-9_]*)|(^[_][a-zA-Z0-9_]+)";
boost::regex Lexical::regex_label (regex_label_str);

Lexical::Lexical()
{
	std::cout << "LOL" << std::endl;
}

Lexical::~Lexical()
{

}

bool Lexical::CheckLabel(std::string label)
{
	try
	{
		if (boost::regex_match (label, regex_label))
			return false;
		else
		{
			log<LOG_ERROR>("Variavel ou Rotulo %1% Invalido.") % label;
			return true;
		}
	}
	catch (boost::regex_error e)
	{
		std::cerr << "Error on label checking. " << e.what() << std::endl;
	}
	return false;
}