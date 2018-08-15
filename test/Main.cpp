#include "Axios/Instance.h"
#include "Axios/DataManager.h"

#include <SFML/Window/Keyboard.hpp>

int main()
{
	ax::DataManager::getInstance().addToDefaultConfig(ax::DataManager::Config("keybinding", "left" , "k71"),false);
	ax::DataManager::getInstance().addToDefaultConfig(ax::DataManager::Config("keybinding", "right", "k72"),false);
	ax::DataManager::getInstance().addToDefaultConfig(ax::DataManager::Config("keybinding", "jump" , "k57"),false);

	ax::Instance::getInstance().init();

	ax::Instance::getInstance().start();

	return 0;
}