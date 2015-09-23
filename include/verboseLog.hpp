#ifndef _VERBOSE_LOG_H_
#define _VERBOSE_LOG_H_

#include <iostream>
#include <string>
#include <boost/format.hpp>

#include "defines.hpp"

class VerboseLog {
	public:
		VerboseLog(std::string msg, verbose_level level);
		~VerboseLog();
		//Operador para permitir a utilização de variaveis.
		//Utilização: log<verbose_level>("Frase exemplo %1% %2% %3% fim da frase") % arg1 % arg2 % arg3;
		template <typename T> 
	    VerboseLog& operator %(T value) {
	        fmt % value;
	        return *this;
	    }    
	private:
		//boost::format é utilizado para permitir a impressão de variáveis usando o overload do operador %.
		boost::format fmt;
		verbose_level level;
};

//Para melhor formatação a classe não será chamada diretamente. A funçao auxiliar a seguir fará esse serviço.
template <verbose_level level>
VerboseLog log(std::string msg)
{
	return VerboseLog(msg, level);
}

#endif //_VERBOSE_LOG_H_