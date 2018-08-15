#include "Axios/Logger.h"
#include "Axios/DataManager.h"

#include <iostream>

void ax::Logger::log(const int verbosity, const char * message, const char * projectName, const MessageType messageType, const char * className)
{
//	if (verbosity > std::stoi(DataManager::Config().data["verbosity"]))
//		return;
//
	// This is so ugly, where is my string formatting???
	std::string string;
	string.append("[");
	string.append(projectName);
	string.append(" ");

	std::string str;
	switch (messageType)
	{
	case ERROR:
		str = "ERROR";
		break;
	case WARNING:
		str = "WARNING";
		break;
	case INFO:
		str = "INFO";
		break;
	}
	string.append(str);

	string.append("] ");
	string.append(className);
	string.append(": ");
	string.append(message);

	std::clog << string << '\n';
}
