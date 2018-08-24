#include "Axios/Scene.h"
#include "Axios/DataManager.h"

void ax::Scene::onEnter()
{
	setTitle(ax::DataManager::getInstance().getConfig("config", "title").c_str(), false);
	updateUserConfig();
}

void ax::Scene::onExit()
{
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
	m_nextScene(nullptr),
	m_filePath(filePath)
{
}
