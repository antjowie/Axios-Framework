#include "Axios\DataManager.h"

ax::DataManager::Data<std::string>& ax::DataManager::Config()
{
	static Data<std::string> instance;
	return instance;
}

ax::DataManager::Data<KeyItem>& ax::DataManager::GameKey()
{
	static Data<KeyItem> instance;
	return instance;
}