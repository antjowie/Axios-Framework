#pragma once
#include <vector>

namespace ax
{
	class Object;
	class Instance;

	// Manages object query, lifetime, updating and serialization
	class ObjectManager
	{
	private:
		friend Instance;
		friend Object;

		std::vector<Object*> m_objects;

		// Destroy the objects that have to be destroyed
		void _clear();

		ObjectManager();

	public:
	
		static ObjectManager& getInstance();
	};
	
	// You inherit from this class if you write an object class
	// This class will add your object to the object manager container
	class Object
	{
	private:	
		friend ObjectManager;

	public:
		// If you change this to true, the object will be removed
		bool m_destroy;
	
		Object();
		virtual ~Object();
	};
}