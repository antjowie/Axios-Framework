#include "Axios/Instance.h"
#include "Axios/DataManager.h"

#include <SFML/Window/Keyboard.hpp>

int main()
{
	ax::DataManager::Config([](const bool overwrite)
	{
		ax::DataManager::GameKey().check("jump",{ sf::Keyboard::Space, -1 }, overwrite);
		ax::DataManager::GameKey().check("left",{ sf::Keyboard::A, -1 }, overwrite);
		ax::DataManager::GameKey().check("right",{ sf::Keyboard::D, -1 }, overwrite);
	});
	
	ax::Instance::getInstance().start();

	return 0;
}