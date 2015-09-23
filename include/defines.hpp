#ifndef _DEFINES_H_
#define _DEFINES_H_

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


extern program_mode PROGRAM_MODE;
extern verbose_level GLOBAL_LEVEL;







#endif //_DEFINES_H_