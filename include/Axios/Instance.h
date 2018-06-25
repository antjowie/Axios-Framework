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
		// This function must be called after user settings have been changed
		void updateUserConfig();
		// Call this function to change the title instead of m_window.setTitle()
		// It saves a copy of the current title so that the use can change 
		// To fullscreen and still have the old title
		void setTitle(const char* title, const bool &append = true);
		void start();

		static Instance& getInstance();
	};
}