#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

#include <string>
#include <vector>

namespace ax
{
	enum class UpdateLoopType
	{
		Real,
		Physics
	};

	class Instance;

	// This class is used for the update loops
	// When the owner object is destroyed, the reference in the
	// container will also be removed, this avoids invalidated pointers.
	class UpdateLoopObject
	{
	private:
		using callbackUpdateLoopFunction = void(*)(const float elapsedTime);
		friend Instance;
		
		callbackUpdateLoopFunction m_function;

		// We hold a reference to an update loop so that we can add as many 
		// update loops as we want. This way, we don't have to check the type
		// every time, we just call the referenced update loop
		std::vector<UpdateLoopObject> *m_updateLoop;

		// The index is used to erase the object when 
		// the owner of this class is destroyes
		int m_index;
		
	public:
	
		// This function directly calls the passed callback function
		void operator()(const float elapsedTime);

		// UpdateLoopType is the type of updateLoop, there are two types
		// Real, is updated every frame
		// Physics, is updated on a specified interval
		// The callbackUpdateLoopFunction is the function that will be called
		// when the update loop is called.
		UpdateLoopObject(const UpdateLoopType updateLoopType, callbackUpdateLoopFunction &function);
		~UpdateLoopObject();
	};
	
	class Instance
	{
	private:
		sf::RenderWindow m_window;
		float m_refreshRate;
		std::string m_currentTitle;
		
		Instance();

		// This section of code is for the update loop
		friend UpdateLoopObject;
		
		std::vector<UpdateLoopObject> m_realUpdateLoop;
		std::vector<UpdateLoopObject> m_physicsUpdateLoop;

	public:
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

		static Instance& getInstance();
	};
}