#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

#include <vector>


namespace ax
{	
	// You inherit from this class if you write an object class
	// This class will add your object to the object manager container
	class ObjectReference;
	class Object
	{
	private:
		friend ObjectReference;
		// These values are assigned by the object reference class
		unsigned int m_referenceIndex;
		unsigned int m_uniqueReferenceID;

		unsigned int m_uniqueID;

	public:
		// The type is used for the draw order.
		// Entities are dependent upon the surface they're
		// walking. It has to be updated in the correct
		// order.
		enum Type
		{
			Platform,
			Vehicle,
			Entity,
			Wearable,
			Hud,
			Count
		};

		// If you change this to true, the object will be removed
		bool m_destroy;

		Object(const Type type);
		virtual ~Object();
	};

	class ObjectManager
	{
	private:
		std::vector<std::vector<Object*>> m_objects;
		unsigned int m_currentUniqueID;
		
		ObjectManager();

	public:
		void _update(const double elapsedTime);
		
		void _physicsUpdate(const double elapsedTime);

		void _draw(sf::RenderWindow & window, const double &accumulator);

		// Destroy the objects that have to be destroyed
		void _clear();
		
		unsigned int _getUniqueID();

		static ObjectManager& getInstance();
	};

	// Instantiate an object of this class to hold a reference.
	// This class is the key used to access the ObjectReferenceManager.
	class ObjectReference
	{
	private:
		const unsigned int m_index;
		const unsigned int m_uniqueID;

		Object* m_object;

	public:
		ObjectReference(Object *object);

		Object* get();
	};

	// Manages object query, lifetime, updating and serialization
	namespace ObjectReferenceManager
	{
		static const unsigned int m_size{ 1024 };
		static Object* m_array[m_size];

		unsigned int _getUniqueID();
		unsigned int _getFirstFreeIndex();
	};
}