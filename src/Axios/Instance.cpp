#include "Axios/Instance.h"
#include "Axios/InputHandler.h"
#include "Axios/DataManager.h"
#include "Axios/Logger.h"
#include "Axios/Object.h"

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

ax::Instance::Instance():
	m_init(false)
{
}

void ax::Instance::updateUserConfig()
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

void ax::Instance::setTitle(const char * title, const bool & append)
{
	if (!append)
		m_currentTitle = title;
	else
		m_currentTitle = ax::DataManager::getInstance().getConfig("config", "title") + " | " + std::string(title);

	if (m_window.isOpen())
		m_window.setTitle(m_currentTitle);
}

void ax::Instance::init()
{
	// Load config data
	ax::DataManager::getInstance()._load("config.json");
	
	// Check framework related config
	ax::DataManager::getInstance().addToDefaultConfig({ "config", "title", "No title" }, false);
	ax::DataManager::getInstance().addToDefaultConfig({ "config", "verbosity", "10" }, false);

	// Check user related config
	ax::DataManager::getInstance().addToDefaultConfig({ "config", "windowX", std::to_string(sf::VideoMode().getDesktopMode().width).c_str() }, false);
	ax::DataManager::getInstance().addToDefaultConfig({ "config", "windowY", std::to_string(sf::VideoMode().getDesktopMode().height).c_str() }, false);
	ax::DataManager::getInstance().addToDefaultConfig({ "config", "fullscreen", "0" }, false);
	ax::DataManager::getInstance().addToDefaultConfig({ "config", "refreshRate", "144" }, false);
	ax::DataManager::getInstance().addToDefaultConfig({ "config", "vsync", "0" }, false);

	// Check system related config
	ax::DataManager::getInstance().addToDefaultConfig({ "config", "physicsPolling" , "100" }, false);

	ax::DataManager::getInstance()._checkConfig(false);

	setTitle(ax::DataManager::getInstance().getConfig("config","title").c_str(), false);
	updateUserConfig();

	m_init = true;
}

void ax::Instance::start()
{
	// Check if user called init function
	if (!m_init)
	{
		ax::Logger::log(5, "Init hasn't been called before start, custom configurations will not be applied", "Axios", ax::Logger::WARNING, "Instance");
		init();
	}

	// These values can't be changed by the user during runtime, that's why they are local to this scope
    sf::Event event;
    
	sf::Clock currentTime; 
	double elapsedTime{0};
	double accumulator{0};

	// Loop phase
    while(m_window.isOpen())
    {
	    elapsedTime = currentTime.restart().asSeconds();
    
		// If the game has a fps lower than 0, skip the frame
        if (elapsedTime > 1)
        {
            elapsedTime = 0;
            continue;
        }

		accumulator += elapsedTime;
		
		ax::InputHandler::getInstance()._update(m_window,event);   
		ax::ObjectManager::getInstance()._update(elapsedTime);
			
		while (accumulator >= m_physicsPolling)
        {
			ax::ObjectManager::getInstance()._physicsUpdate(m_physicsPolling);
			accumulator -= m_physicsPolling;
        }

		m_window.clear();

		ax::ObjectManager::getInstance()._draw(m_window, accumulator);

		m_window.display();
    }

	// Deinit phase
	ax::DataManager::getInstance()._save("config.json");
}

ax::Instance & ax::Instance::getInstance()
{
	static Instance instance;
	return instance;
}