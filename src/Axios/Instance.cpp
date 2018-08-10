#include "Axios/Instance.h"
#include "Axios/InputHandler.h"
#include "Axios/DataManager.h"
#include "Axios/Logger.h"

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <sstream>

float getAverage(const std::list<float> & list)
{
	float total{ 0 };
	for (const auto &ms : list)
		total += ms;
	return total / list.size();
}

ax::Instance::Instance():
	m_init(false)
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

void ax::Instance::init()
{
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

	ax::DataManager::GameKey()._check(false);

	setTitle(ax::DataManager::Config().data["title"].c_str(), false);
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

			//m_realUpdateLoop._update(averageTime.getAverage());
			m_realUpdateLoop._update(elapsedTime);

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

ax::UpdateLoopObject::UpdateLoopObject(const UpdateLoopType updateLoopType, const UpdateFunction & updateFunction):
	m_function(updateFunction)
{
	// Get the correct update loop
	switch (updateLoopType)
	{
	case UpdateLoopType::Real:
		m_updateLoop = &ax::Instance::getInstance().m_realUpdateLoop;
		break;
	case UpdateLoopType::Physics:
		m_updateLoop = &ax::Instance::getInstance().m_physicsUpdateLoop;
		break;
	default:
		Logger::log(0, "UpdateLoopType not specified", "Axios", ax::Logger::ERROR, "UpdateLoopObject");
		break;
	}
	m_index = m_updateLoop->m_updateLoop.size();

	// Push the object into the update loop
	m_updateLoop->m_updateLoop.push_back(*this);

	// Test code
	std::stringstream ss;
	ss << static_cast<const void*>(this);
	ax::Logger::log(0, ss.str().c_str(), "Axios", ax::Logger::INFO, "UpdateLoopObject");
}

ax::UpdateLoopObject::~UpdateLoopObject()
{
	m_updateLoop->m_destroyIndexes.push_back(m_index);
}

void ax::Instance::UpdateLoop::_update(const float elapsedTime)
{
	m_elapsedTime += elapsedTime;
	
	// Checks if objects should be updated
	if (m_interval != 0 && m_elapsedTime == std::fmod(m_elapsedTime, m_interval))
		return;
	
	const float modulo = (m_interval == 0) ? m_elapsedTime : m_interval;
	
	do
	{
		for (auto iter : m_updateLoop)
			iter.get().m_function(modulo);

		m_elapsedTime -= modulo;
	} while (m_elapsedTime > modulo);
}

void ax::Instance::UpdateLoop::_clear()
{
	for (const auto &iter : m_destroyIndexes)
	{
		// Update the index
		m_updateLoop.back().get().m_index = m_updateLoop[iter].get().m_index;
		
		// Remove the element
		std::swap(m_updateLoop[iter], m_updateLoop.back());
		m_updateLoop.pop_back();
	}

	m_destroyIndexes.clear();
}

ax::Instance::UpdateLoop::UpdateLoop(const float interval):
	m_interval(interval),m_elapsedTime(0)
{
}