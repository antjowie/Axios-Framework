#pragma once

namespace ax
{
	namespace Logger
	{
		// The lower the verbosity, the more important the message is
		void log(const char* message, const int verbosity);
	}
}