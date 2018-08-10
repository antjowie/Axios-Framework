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

	class UpdateLoopObject;
	
	class Instance
	{
	private:
		sf::RenderWindow m_window;
		float m_refreshRate;
		std::string m_currentTitle;
		
		Instance();

		// This section of code is for the update loop
		friend UpdateLoopObject;
		
		class UpdateLoop
		{
		// This can only be accessed when a class had access to the Instance class.
		// The public modifier serves no problem
		public:
			// This keeps track of the elapsedTime, if the value changes,
			// update all objects in the update vector
			const float m_interval;
			float m_elapsedTime;

			// These vectors contain the objects to update whatever
			// code requests it.
			std::vector<std::reference_wrapper<UpdateLoopObject>> m_update;

			// Checks if the objects have to be updated
			// If interval has been declared in the constructor,
			// it will only be updated if that time has elapsed.
			// Else it will update every frame
			void _update(const float elapsedTime);

			// Removed invalidated objects using the
			// destroy vector
			void _clear();

			// If the interval is not changed, the update loop
			// will update the objects every frame
			UpdateLoop(const float interval = 0);
		};
		
		UpdateLoop m_realUpdateLoop;
		UpdateLoop m_physicsUpdateLoop;

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

	// This class is used for the update loops
	// When the owner object is destroyed, the reference in the
	// container will also be removed, this avoids invalidated pointers.
	class UpdateLoopObject
	{
	private:
		using updateLoopFunction = void(*)(const float elapsedTime);
		friend Instance;

		updateLoopFunction m_function;

		// We hold a reference to an update loop so that we can add as many 
		// update loops as we want. This way, we don't have to check the type
		// every time, we just call the referenced update loop
		Instance::UpdateLoop *m_updateLoop;

		// The bool is used to erase the object when 
		// the owner of this class is destroyes
		bool m_destroyed;

	public:

		// This function directly calls the passed callback function
		void operator()(const float elapsedTime);

		// UpdateLoopType is the type of updateLoop, there are two types
		// Real, is updated every frame
		// Physics, is updated on a specified interval
		// updateLoopFunction: void (const float)
		UpdateLoopObject(const UpdateLoopType updateLoopType, updateLoopFunction &function);
		~UpdateLoopObject();
	};
}