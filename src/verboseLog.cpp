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
				lvlString = std::string("Erro");
				break;
			case LOG_WARNING:
				lvlString = std::string("Warning");
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
		std::cout << "[" << lvlString << " " << type << "] " << fmt << std::endl;
	}
}