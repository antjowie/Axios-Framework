#include "Axios\DataManager.h"

ax::DataManager::Data<std::string>& ax::DataManager::Config(const callbackFunction &extraCheck)
{
	static Data<std::string> instance(extraCheck);
	return instance;
}

ax::DataManager::Data<KeyItem>& ax::DataManager::GameKey(const callbackFunction &extraCheck)
{
	static Data<KeyItem> instance(extraCheck);
	return instance;
}