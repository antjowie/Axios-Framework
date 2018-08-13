#include "Axios/Object.h"
#include "Axios/Logger.h"

#include <algorithm>

ax::ObjectManager::ObjectManager():
	m_currentUniqueID(0)
{
	m_objects.resize(Object::Type::Count);
}

void ax::ObjectManager::_update(const double elapsedTime)
{
}

void ax::ObjectManager::_physicsUpdate(const double elapsedTime)
{
}

void ax::ObjectManager::_draw(sf::RenderWindow & window, const double & accumulator)
{
}

void ax::ObjectManager::_clear()
{	
	for(auto &deleter : m_objects)
	deleter.erase(std::remove_if(deleter.begin(),deleter.end(),
		[](Object *object)
	{
		bool state = (object == nullptr);
		if (state)
			delete object;
		return state;
	}
	),deleter.end());
}

unsigned int ax::ObjectManager::_getUniqueID()
{
	return m_currentUniqueID++;
}

ax::ObjectManager & ax::ObjectManager::getInstance()
{
	static ObjectManager objectManager;
	return objectManager;
}

ax::Object::Object(const Type type):
	m_uniqueID(ax::ObjectManager::getInstance()._getUniqueID()),
	m_uniqueReferenceID(ax::ObjectReferenceManager::_getUniqueID()),
	m_referenceIndex(ax::ObjectReferenceManager::_getFirstFreeIndex()),
	m_destroy(false)
{
	ax::Logger::log(10, std::string("Object " + std::to_string(m_uniqueID) + " created").c_str(), "Axios",Logger::INFO,"Object");
}

ax::Object::~Object()
{
	ax::Logger::log(10, std::string("Object " + std::to_string(m_uniqueID) + " destroyed").c_str(), "Axios", Logger::INFO, "Object");
}

unsigned int ax::ObjectReferenceManager::_getUniqueID()
{
	static unsigned int ID{ 0 };
	return ID++;
}

unsigned int ax::ObjectReferenceManager::_getFirstFreeIndex()
{
	unsigned int index{ 0 };
	while (index < m_size)
		if (m_array[index] == nullptr)
			return index;

	ax::Logger::log(0, "Maximum Object count surpassed", "Axios", ax::Logger::ERROR, "ObjectReferenceManager");
	return -1;
}

ax::ObjectReference::ObjectReference(Object * object) :
	m_index(object->m_referenceIndex), m_uniqueID(object->m_uniqueReferenceID)
{
}