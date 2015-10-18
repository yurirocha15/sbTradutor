#include "verboseLog.hpp"

VerboseLog::VerboseLog(const std::string& msg, const verbose_level& level, const std::string& type) : fmt(msg), level(level), type(type)
{
}

VerboseLog::~VerboseLog()
{
	//GLOBAL_LEVEL is created at defines.h file and is defined depending on the runtime options.
	if (level <= GLOBAL_LEVEL)
	{
		std::string lvlString;
		switch(level)
		{
			case LOG_ERROR:
				lvlString = std::string("\033[1;31mErro \033[0m");
				break;
			case LOG_WARNING:
				lvlString = std::string("\033[1;33mWarning \033[0m");
				break;
			case LOG_INFO:
				lvlString = std::string("INFO");
				break;
			case LOG_DEBUG:
				lvlString = std::string("DEBUG");
				break;
			case LOG_NOTHING:
				lvlString = std::string(" ");

		}
		std::cout << "[" << lvlString << "\033[1;31m" << type << "\033[0m] " << fmt << std::endl;
	}
}