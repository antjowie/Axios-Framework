#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

#include <string>
#include <vector>

namespace ax
{
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
			
			// The unique id generator. Doesn't need any special code.
			// This value increases every time a new object has been
			// pushed into the container.
			static unsigned int m_id;

			// These vectors contain the objects to update whatever
			// code requests it.
			std::vector<UpdateLoopObject*> m_updateLoop;

			// This vector contains the indexes to be destroyed
			std::vector<int> m_destroyIds;

			// Checks if the objects have to be updated
			// If interval has been declared in the constructor,
			// it will only be updated if that time has elapsed.
			// Else it will update every frame
			void _update(const float elapsedTime);

			// Removed invalidated objects using the
			// destroy vector
			void _clear();
		};
		
		UpdateLoop m_realUpdateLoop;
		UpdateLoop m_physicsUpdateLoop;

		bool m_init;

	public:
		// This function must be called after user settings have been changed
		// Updates window if player changes to fullscreen
		void updateUserConfig();

		// Call this function to change the title instead of m_window.setTitle()
		// It saves a copy of the current title so that the user can change 
		// to fullscreen and still have the old title
		void setTitle(const char* title, const bool &append = true);
		
		// Initiates all the systems
		// !!! If you want to add custom config 
		// !!! you should first call the constructors
		// !!! of the data manager class
		void init();

		// Starts the main gameloop
		// !!! Run function init() first
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
		friend Instance;

		// Every object has an unique ID, so that it can be removed from
		// the container to avoid invalidated pointers
		const unsigned int m_id;

		// Used for the destructor
		bool m_hookedToUpdate;
		bool m_hookedToPhysicsUpdate;

	public:
		// This function gets called when the real update loop
		// gets iterated.
		// !!! If you want to use this function, make a call
		// !!! too hookUpdate first
		virtual void update(const float elapsedTime);

		// This function gets called when the physics update loop
		// gets iterated.
		// !!! If you want to use this function, make a call
		// !!! too hookPhysicsUpdate first
		virtual void physicsUpdate(const float elapedTime);


		// Assigns the updateLoopObject an unique ID
		UpdateLoopObject();

		virtual ~UpdateLoopObject();
	};
}