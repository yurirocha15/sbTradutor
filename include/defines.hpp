#ifndef _DEFINES_HPP_
#define _DEFINES_HPP_

//Esse arquivo contem os defines e variáveis globais
enum program_mode
{
	MODE_NOTHING,
	MODE_PRE,
	MODE_MACRO,
	MODE_OUTPUT
};

//Nível de verbosidade
enum verbose_level {
	LOG_NOTHING = 0,
	LOG_ERROR,
	LOG_WARNING,
	LOG_INFO,
	LOG_DEBUG
};


//Como variáveis nao podem ser definidas em um arquivo .hpp, elas são declaradas extern e definidas no arquivo defines.cpp
extern program_mode PROGRAM_MODE;
extern verbose_level GLOBAL_LEVEL;
extern bool THERE_WAS_ERROR;







#endif //_DEFINES_HPP_