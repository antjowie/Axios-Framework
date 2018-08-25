#include "Axios/Instance.h"
#include "Axios/InputHandler.h"
#include "Axios/DataManager.h"
#include "Axios/Object.h"

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

ax::Instance::Instance() :
	m_init(false),
	m_currentScene(nullptr)
{
}

void ax::Instance::start()
{
	// Check if user called init function
	if (!m_init)
	{
		ax::Logger::log(0, "Init hasn't been called before start, custom configurations will not be applied", "Axios", ax::Logger::ERROR, "Instance");
		return;
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

		// Check if scene has to be swapped
		if (m_currentScene->m_nextScene != nullptr)
		{
			// Keep address of old scene
			Scene * toDelete = m_currentScene;
			m_currentScene->onExit();
			
			// Swap scene
			m_currentScene = m_currentScene->m_nextScene;
			delete toDelete;
			m_currentScene->onEnter();
		}

		ax::InputHandler::getInstance()._update(m_window,event);   
	
		m_currentScene->_update(elapsedTime);	

		m_window.clear();
		 
		m_currentScene->_draw(m_window, accumulator);

		m_window.display();
    }

	// Deinit phase
	delete m_currentScene;
	ax::DataManager::getInstance()._checkConfig(false);
	ax::DataManager::getInstance()._save("config.json");
}