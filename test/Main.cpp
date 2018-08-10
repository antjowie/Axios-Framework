#include "Axios/Instance.h"
#include "Axios/DataManager.h"

#include "Axios/Logger.h"

#include <SFML/Window/Keyboard.hpp>

struct Temp: ax::UpdateLoopObject
{
	int count{ 0 };
	
	void function(const float elapsedTime)
	{
		count += 1;
		std::stringstream ss;
		ss << static_cast<const void*>(this);
		ax::Logger::log(0, ss.str().c_str(), "Axios", ax::Logger::INFO, "UpdateLoop");

		ax::Logger::log(0, std::to_string(count).c_str(), "Axios", ax::Logger::INFO, "UpdateLoop");
	}

	// I'm stuck. How can I pass a member function as function pointer?
	Temp():
		ax::UpdateLoopObject(ax::UpdateLoopType::Real, [this](const float elapsedTime)
	{
		count += 1;
		std::stringstream ss;
		ss << static_cast<const void*>(this);
		ax::Logger::log(0, ss.str().c_str(), "Axios", ax::Logger::INFO, "UpdateLoop");

		ax::Logger::log(0, std::to_string(count).c_str(), "Axios", ax::Logger::INFO, "UpdateLoop");

	}
		)
	{

	}
};

int main()
{
	ax::DataManager::Config([](const bool overwrite)
	{
		ax::DataManager::GameKey().check("jump",{ sf::Keyboard::Space, -1 }, overwrite);
		ax::DataManager::GameKey().check("left",{ sf::Keyboard::A, -1 }, overwrite);
		ax::DataManager::GameKey().check("right",{ sf::Keyboard::D, -1 }, overwrite);
	});
	
	ax::Instance::getInstance().init();
	
	ax::Instance::getInstance().start();

	return 0;
}