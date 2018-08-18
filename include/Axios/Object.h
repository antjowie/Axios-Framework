#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <vector>
#include <unordered_map>

namespace ax
{
	const static unsigned int maxObjectCount = 1024;

	class Object;
	class ObjectManager;
	class ObjectFactory;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Manages object query, lifetime, updating and serialization
	class ObjectReferenceManager
	{
	public:
		Object* m_array[maxObjectCount];

		// Gets a unique index
		unsigned int _getUniqueID();

		// Gets the first free index slot in the container
		unsigned int _getFirstFreeIndex() const;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Instantiate an object of this class to hold a reference.
	// This class is the key used to access the ObjectReferenceManager.
	class ObjectReference
	{
	private:
		const unsigned int m_referenceIndex;
		const unsigned int m_uniqueReferenceID;

		ObjectReferenceManager &m_referenceManager;
	public:
		ObjectReference(Object *object);

		// Returns nullptr if object doesn't exist anymore
		Object* get();
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	// You instantiate from this class if you write an object class
	// This class will add your object to the object manager container
	class Object : public sf::Drawable
	{
	private:
		friend ObjectManager;
		// If you change this to true, the object will be removed
		// !!! You should use the destroy() function instead
		bool m_destroy;

	protected:
		friend ObjectReference;
		// These values are assigned by the object reference class.
		// Access to these members is not neccessarily, but could be useful
		// for debugging purposes
		const unsigned int m_referenceIndex;
		const unsigned int m_uniqueReferenceID;

		// Use this member to make new objects
		ObjectFactory &m_objectFactory;

		// Overwrite this function to declare draw behavior
		virtual void draw(sf::RenderTarget &window, sf::RenderStates states) const override;

		// The time till destruction
		// -1 means it is not yet activated
		float m_timeTillDestroy = -1;

	public:
		// The type is used for the draw order.
		// Entities are dependent upon the surface they're
		// walking. It has to be updated in the correct
		// order.
		const enum Type
		{
			Platform,
			Vehicle,
			Entity,
			Wearable,
			Hud,
			Count
		}m_type;

		// !!! Overwrite this function if you want to add update
		// !!! behavior to the object. Be sure to call
		// !!! the base update function first before adding
		// !!! your own code to the function because it manages
		// !!! the timeline for object removal.
		virtual void update(const double elapsedTime);

		// 0 makes the object be destroyed in the same frame
		void destroy(float timeTillDestroy);

		Object(ObjectFactory &objectFactory, const Type type);

		virtual ~Object();
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	// This class manages all the objects.
	// Their lifetime, allocation and references.
	class ObjectManager
	{
	private:
		std::vector<std::vector<Object*>> m_objects;

	public:
		// ObjectReference needs access to this member variable
		// Object uses this variable to assign an unique id and 
		// a free slot.
		ax::ObjectReferenceManager _m_referenceManager;

		// Updates all the objects with the elapsedTime
		void _update(const double elapsedTime);

		// Draws all the objects to the given window.
		// accumulator is used to create intermidiate positions
		// for the physics objects
		void _draw(sf::RenderWindow & window, const double &accumulator);

		// Used to push objects onto the vector
		// and the referenceManager.
		void _add(Object* object);

		// Destroy the objects that have to be destroyed
		void _clear();

		ObjectManager();
		~ObjectManager();
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	// ObjectFactory overwrite this function
	template <class ObjectType>
	Object* _createObject(ObjectFactory & objectFactory) { return new ObjectType(objectFactory); };

	// The object factory constructs objects for you by passing in a string
	// You should first fill the object factory in the onEnter function of scene
	class ObjectFactory
	{
	private:
		// This map contains all the constructor methods
		std::unordered_map<const char*, Object* (*)(ObjectFactory&)> m_map;
		
	public:
		template <class ObjectType>
		void add(const char* name)
		{
			m_map[name] = &_createObject<ObjectType>;
		}

		// Constructs an object.
		Object* construct(const char* name)
		{
			Object * object = m_map[name](*this);
			_m_objectManager._add(object);
			return object;
		}

		// This is public to make sure the object 
		// constructor parameters stays
		// less bloated. ObjectFactory needs access
		// to this because it pushes the new objects
		ObjectManager &_m_objectManager;

		ObjectFactory(ObjectManager & objectManager);
	};
}