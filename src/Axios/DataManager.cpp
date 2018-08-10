#include "Axios\DataManager.h"

ax::DataManager::Data<std::string>& ax::DataManager::Config(const extraCheck &extraCheck)
{
	static Data<std::string> instance(extraCheck);
	return instance;
}

ax::DataManager::Data<KeyItem>& ax::DataManager::GameKey(const extraCheck &extraCheck)
{
	static Data<KeyItem> instance(extraCheck);
	return instance;
}