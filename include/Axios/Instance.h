#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

#include <string>

namespace ax
{
	class Instance
	{
	private:
		
		sf::RenderWindow m_window;
		float m_refreshRate;
		std::string m_currentTitle;

		Instance();

	public:
		using callbackUpdateLoop = void(*)(float elapsedTime);
		// This function must be called after user settings have been changed
		// Updates window if player changes to fullscreen
		void updateUserConfig();

		// Call this function to change the title instead of m_window.setTitle()
		// It saves a copy of the current title so that the user can change 
		// to fullscreen and still have the old title
		void setTitle(const char* title, const bool &append = true);
		
		// Starts the main gameloop
		// !!! If you want to add custom config 
		// !!! you should first call the constructors
		// !!! of the data manager class
		void start();

		void callbackRealUpdateLoop(callbackUpdateLoop & function);

		void callbackPhysicsUpdateLoop(callbackUpdateLoop & function);

		static Instance& getInstance();
	};
}