#pragma once
#include "Axios/Object.h"

#include <SFML/Graphics/RenderWindow.hpp>

namespace ax
{
	// Inherit from this scene to make your own scene
	// The scene class is a container for a level.
	class Scene
	{
	private:
		ax::ObjectManager m_objectManager;
		
		sf::RenderWindow &m_window;

		// SFML doesn't have a function like getTitle, 
		// so we need to keep the title stored
		std::string m_currentTitle;	

		double m_accumulator;	// Accumulates the elapsed time for physics objects
		double m_timeModifier;	// Modifies the time

		float m_physicsPolling;	// The rate at which the physics engine runs
	
	protected:
		ax::ObjectFactory m_objectFactory;

		const char *m_filePath;
	
	public:
		// Change this from nullptr to swap the scene next frame
		Scene * m_nextScene;

		// Called when this scene is pushed
		// !!! This function updates the title and applies 
		// !!! any custom user configurations. If you overwrite
		// !!! This function (which you must because you have to
		// !!! implement level loading), be sure to call this
		// !!! function at the end of your overwritten function
		// !!! or call the functions manually
		virtual void onEnter();

		// Called when this scene is removed
		// !!! This functions body is empty.
		// !!! It is reccomended to implement something
		// !!! like level saving in this function.
		virtual void onExit();

		// Updates the objects
		void _update(const double elapsedTime);

		// Displays all the objects that are drawn during the frame
		void _draw(sf::RenderWindow &window, const double accumulator);

		// This function must be called after user settings have been changed
		// Updates window if player changes to fullscreen
		void updateUserConfig();

		// Call this function to change the title instead of m_window.setTitle()
		// It saves a copy of the current title so that the user can change 
		// to fullscreen and still have the old title
		void setTitle(const char* title, const bool &append = true);

		// !!! Always leave the body of this constuctor empty
		Scene(sf::RenderWindow& window, const char* filePath);
	};
}