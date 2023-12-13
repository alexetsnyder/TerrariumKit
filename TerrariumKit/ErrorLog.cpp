#include "ErrorLog.h"

#include <iostream>

void logError(const std::string& caller, const std::string& errorMessage)
{
	std::cerr << "Error::" << caller;
	if (errorMessage != "" && errorMessage != "\n")
	{
		std::cerr << "::" << errorMessage;
	}
	std::cerr << std::endl;
}