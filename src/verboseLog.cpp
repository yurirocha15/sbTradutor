#include "verboseLog.hpp"

VerboseLog::VerboseLog(std::string msg, verbose_level level) : fmt(msg), level(level)
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
				lvlString = std::string("ERROR");
				break;
			case LOG_WARNING:
				lvlString = std::string("WARNING");
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
		std::cout << "[" << lvlString << "] " << fmt << std::endl;
	}
}