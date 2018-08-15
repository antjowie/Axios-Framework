#include "Axios/DataManager.h"
#include "Axios/Logger.h"

void ax::DataManager::addToDefaultConfig(const Config & config, const bool overwriteOnOccurance)
{
	auto swapIter{ std::find_if(m_control.begin(),m_control.end(),[&](Config iter)
	{
		return (config.category == iter.category && config.key == iter.key);
	})};
	
	if (swapIter == m_control.end())
		m_control.push_back(config);
	else if (overwriteOnOccurance)
	{
		m_control.push_back(config);
		std::iter_swap(swapIter, m_control.end()-1);
		m_control.pop_back();
	}
}

void ax::DataManager::_checkConfig(bool overwriteToDefault)
{
	std::unordered_map<std::string,
		std::unordered_map<std::string, std::string>> newData;

	for (const auto &iter : m_control)
	{
		newData[iter.category][iter.key] = iter.value;
		if (!overwriteToDefault && m_data.count(iter.category) != 0 && m_data.at(iter.category).count(iter.key) != 0)
			newData[iter.category][iter.key] = m_data[iter.category][iter.key];
	}

	m_data = std::move(newData);
}

std::string ax::DataManager::getConfig(const char * category, const char * key) const
{
	if (m_data.count(category) != 0 && m_data.at(category).count(key) != 0)
		return m_data.at(category).at(key);
	return "-1";
}

void ax::DataManager::setConfig(const Config & config)
{
	m_data[config.category][config.key] = config.value;
}

void ax::DataManager::_load(const char * path)
{
	json j;
	std::ifstream configFile(path);
	if (!configFile.is_open())
	{
		ax::Logger::log(0, std::string("Config file couldn't be found at path: " + std::string(path)).c_str(), "Axios", Logger::ERROR, "DataManager");
		return;
	}

	configFile >> j;

	for (json::iterator category = j.begin(); category != j.end(); ++category)
		for (json::iterator key = category.value().begin(); key != category.value().end(); ++key)
			m_data[category.key()][key.key()] = key.value().get<std::string>();
}

void ax::DataManager::_save(const char * path)
{
	std::ofstream configFile(path, std::ios::trunc);
	json config(m_data);
	configFile << config.dump(2);
	configFile.close();
}

ax::DataManager & ax::DataManager::getInstance()
{
	static DataManager instance;
	return instance;
}

ax::DataManager::Config::Config(const char * category, const char * key, const char * value):
	category(category),key(key),value(value)
{
}