#include "Axios/Scene.h"
#include "Axios/DataManager.h"

void ax::Scene::onEnter()
{
	// Update window settinsg
	setTitle(ax::DataManager::getInstance().getConfig("config", "title").c_str(), false);
	updateUserConfig();

	// Load level data
	using json = nlohmann::json;
	std::ifstream file("save/" + m_filePath);
	if (!file.is_open())
	{
		ax::Logger::log(5, std::string("Couldn't find save file for scene " + m_currentTitle).c_str(),
			"Axios", ax::Logger::INFO, "Scene");
		file.open(m_filePath);
		if (!file.is_open())
		{
			ax::Logger::log(0, std::string("Couldn't find level file of scene. " + std::string(m_filePath) + " doesn't exist").c_str(),
				"Axios", ax::Logger::MessageType::ERROR, "Scene");
			return;
		}
	}

	json data;
	file >> data;
	file.close();

	std::vector<std::unordered_map<std::string, std::string>> objects;

	for (json::iterator objectData = data.begin(); objectData != data.end(); ++objectData)
	{
		std::unordered_map<std::string, std::string> dataMap;
		for (json::iterator key = objectData.value().begin(); key != objectData.value().end(); ++key)
			dataMap[key.key()] = key.value().get<std::string>();
		objects.push_back(dataMap);
	}

	m_objectManager._load(objects);
}

void ax::Scene::onExit()
{
	using json = nlohmann::json;
	json data;
	
	for (const auto &object : m_objectManager._save())
		data.push_back(object);

	std::ofstream save("save/" + m_filePath);
	
	save << data.dump(2);
	save.close();
}

void ax::Scene::_update(const double elapsedTime)
{
	m_accumulator += elapsedTime;

	m_objectManager._update(elapsedTime);

	while (m_accumulator >= m_physicsPolling)
	{
		//	ax::ObjectManager::getInstance()._physicsUpdate(m_physicsPolling);
		m_accumulator -= m_physicsPolling;
	}

	m_objectManager._clear();
}

void ax::Scene::_draw(sf::RenderWindow & window, const double accumulator)
{
	m_objectManager._draw(window, accumulator);
}

void ax::Scene::updateUserConfig()
{
	// Fix the fullscreen
	m_window.create(
		sf::VideoMode(std::stoi(ax::DataManager::getInstance().getConfig("config", "windowX")), std::stoi(ax::DataManager::getInstance().getConfig("config", "windowY"))),
		m_currentTitle, ax::DataManager::getInstance().getConfig("config", "fullscreen") == "1" ? sf::Style::Fullscreen : sf::Style::Default);

	// Enable or disable the vsync
	if (ax::DataManager::getInstance().getConfig("config", "vsync") == "1")
	{
		m_window.setFramerateLimit(std::stoi(ax::DataManager::getInstance().getConfig("config", "refreshRate")));
		m_window.setVerticalSyncEnabled(true);
	}
	else
	{
		m_window.setVerticalSyncEnabled(false);
		m_window.setFramerateLimit(std::stoi(ax::DataManager::getInstance().getConfig("config", "refreshRate")));
	}

	// Update the physics engine polling rate
	m_physicsPolling = 1.f / std::stof(ax::DataManager::getInstance().getConfig("config", "physicsPolling"));
}

void ax::Scene::setTitle(const char * title, const bool & append)
{
	if (!append)
		m_currentTitle = title;
	else
		m_currentTitle = ax::DataManager::getInstance().getConfig("config", "title") + " | " + std::string(title);

	if (m_window.isOpen())
		m_window.setTitle(m_currentTitle);
}

ax::Scene::Scene(sf::RenderWindow & window, const char* filePath):
	m_window(window),
	m_objectFactory(m_objectManager),
	m_objectManager(m_objectFactory),
	m_nextScene(nullptr),
	m_filePath(filePath)
{
}
