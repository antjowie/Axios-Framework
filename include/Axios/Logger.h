#pragma once

namespace ax
{
	namespace Logger
	{
		enum MessageType
		{
			ERROR,
			WARNING,
			INFO
		};

		// Format: [%projectName% %messageType%] %className%: %message%
		// verbosity, importance, the lower, the more important
		// message, the message to log
		// projectName, name of the project (Axios)
		// messageType, type of message (Error, Warn)
		// className, name of the class (Instance)
		void log(const int verbosity, const char* message, const char *projectName = "null",
			const MessageType messageType = MessageType::INFO, const char *className = "null");
	}
}