#include "Axios/Logger.h"
#include "Axios/DataManager.h"

#include <iostream>

void ax::Logger::log(const char * message, const int verbosity)
{
	if (verbosity <= std::stoi(DataManager::Config().data["verbosity"]))
		std::cout << message << '\n';
}
