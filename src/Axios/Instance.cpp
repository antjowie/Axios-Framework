#include "Axios/Instance.h"
#include "Axios/InputHandler.h"
#include "Axios/DataManager.h"

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

float getAverage(const std::list<float> & list)
{
	float total{ 0 };
	for (const auto &ms : list)
		total += ms;
	return total / list.size();
}

ax::Instance::Instance()
{
}

void ax::Instance::updateUserConfig()
{
	m_window.create(sf::VideoMode(std::stoi(ax::DataManager::Config().data["windowX"]), std::stoi(ax::DataManager::Config().data["windowY"])), 
		m_currentTitle, ((ax::DataManager::Config().data["fullscreen"] == "1") ? sf::Style::Fullscreen : sf::Style::Default));
	m_refreshRate = std::stof(ax::DataManager::Config().data["refreshRate"]);
}

void ax::Instance::setTitle(const char * title, const bool & append)
{
	if (!append)
		m_currentTitle = title;
	else
		m_currentTitle = ax::DataManager::Config().data["title"] + " | " + std::string(title);

	if (m_window.isOpen())
		m_window.setTitle(m_currentTitle);
}

void ax::Instance::start()
{
	// Init phase
	// Load config data
	ax::DataManager::Config()._load("config/config.json");
	ax::DataManager::GameKey()._load("config/keybindings.json");

	// Check framework related config
	ax::DataManager::Config().check("title", "No title");
	ax::DataManager::Config().check("verbosity", "10");

	// Check user related config
	ax::DataManager::Config().check("windowX", std::to_string(sf::VideoMode().getDesktopMode().width));
	ax::DataManager::Config().check("windowY", std::to_string(sf::VideoMode().getDesktopMode().height));
	ax::DataManager::Config().check("fullscreen", "0");
	ax::DataManager::Config().check("refreshRate", "144");

	ax::DataManager::GameKey()._check();

	setTitle(ax::DataManager::Config().data["title"].c_str(), false);
	updateUserConfig();

	// These values can't be changed by the user during runtime, that's why they are local to this scope
    std::list<float> averageTime; // A class that keeps track of the time elapsed between reset calls
    sf::Clock frameTime; 
	sf::Event event;
    float elapsedTime;
    elapsedTime = frameTime.restart().asSeconds();

	// -----
	// Loop phase
    while(m_window.isOpen())
    {
        // If the game has a fps lower than 0, skip the frame
        if (elapsedTime > 1)
        {
            elapsedTime = 0;
            continue;
        }

		if (elapsedTime >= 1.f / m_refreshRate)
        {
			ax::InputHandler::getInstance()._update(m_window,event);   
			averageTime.push_front(elapsedTime);
			if (averageTime.size() > 5) // Max elements
				averageTime.pop_back();

            elapsedTime = std::fmod(elapsedTime, 1.f / m_refreshRate);
			
			m_window.clear();

            //updateScene(averageTime.getAverage());
			
			m_window.display();
        }
        elapsedTime += frameTime.restart().asSeconds();
    }

	// -----
	// Deinit phase
	ax::DataManager::Config()._save("config/config.json");
	ax::DataManager::GameKey()._save("config/keybindings.json");
}

ax::Instance & ax::Instance::getInstance()
{
	static Instance instance;
	return instance;
}
