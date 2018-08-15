#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

#include <string>
#include <vector>

namespace ax
{
	class Instance
	{
	private:
		sf::RenderWindow m_window;
		std::string m_currentTitle;
		float m_physicsPolling;
		bool m_init;

		Instance();

	public:
		// This function must be called after user settings have been changed
		// Updates window if player changes to fullscreen
		void updateUserConfig();

		// Call this function to change the title instead of m_window.setTitle()
		// It saves a copy of the current title so that the user can change 
		// to fullscreen and still have the old title
		void setTitle(const char* title, const bool &append = true);

		// Initiates all the systems.
		// !!! Add your custom config
		// !!! before this function.
		void init();

		// Starts the main gameloop
		// !!! Run function init() first
		// !!! If you want to add custom config 
		// !!! you should first call the constructors
		// !!! of the data manager class
		void start();

		static Instance& getInstance();
	};
}