#pragma once
#include "Axios/Scene.h"
#include "Axios/Logger.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <string>
#include <vector>

namespace ax
{
	class Instance
	{
	private:
		sf::RenderWindow m_window;
		bool m_init;

		Scene* m_currentScene;
		
	public:
		// Initiates all the systems.
		// !!! Add your custom config
		// !!! before this function.
		template <class SceneType>
		void init();

		// Starts the main gameloop
		// !!! Run function init() first
		// !!! If you want to add custom config 
		// !!! you should first call the constructors
		// !!! of the data manager class
		void start();

		Instance();
	};

	template<class SceneType>
	inline void Instance::init()
	{
		// Load config data
		ax::DataManager::getInstance()._load("config.json");

		// Check framework related config
		ax::DataManager::getInstance().addToDefaultConfig({ "config", "verbosity", "10" }, false);
		ax::DataManager::getInstance().addToDefaultConfig({ "config", "physicsPolling" , "100" }, false);


		// Check user related config
		ax::DataManager::getInstance().addToDefaultConfig({ "config", "windowX", std::to_string(sf::VideoMode().getDesktopMode().width).c_str() }, false);
		ax::DataManager::getInstance().addToDefaultConfig({ "config", "windowY", std::to_string(sf::VideoMode().getDesktopMode().height).c_str() }, false);
		ax::DataManager::getInstance().addToDefaultConfig({ "config", "fullscreen", "0" }, false);
		ax::DataManager::getInstance().addToDefaultConfig({ "config", "refreshRate", "144" }, false);
		ax::DataManager::getInstance().addToDefaultConfig({ "config", "vsync", "0" }, false);

		// Check game related config
		ax::DataManager::getInstance().addToDefaultConfig({ "config", "title", "No title" }, false);
		if (ax::DataManager::getInstance().getConfig("config", "firstScenePath") == "-1")
		{
			ax::Logger::log(0, "Configuration firstScenePath not specified. Cannot start game", "Axios", ax::Logger::ERROR, "Instance");
			ax::DataManager::getInstance().setConfig({ "config", "firstScenePath", "/" });
			ax::DataManager::getInstance()._save("config.json");
			return;
		}
		ax::DataManager::getInstance().addToDefaultConfig({ "config", "firstScenePath", "/" }, false);

		ax::DataManager::getInstance()._checkConfig(false);

		m_currentScene = new SceneType(m_window, ax::DataManager::getInstance().getConfig("config", "firstScenePath").c_str());
		m_currentScene->onEnter();

		m_init = true;
	}
}