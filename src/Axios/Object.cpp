#include "Axios/Object.h"
#include "Axios/Logger.h"

#include <algorithm>

ax::ObjectManager::ObjectManager()
{
}

void ax::ObjectManager::_clear()
{	
	m_objects.erase(std::remove_if(m_objects.begin(),m_objects.end(),
		[](Object *object)
	{
		bool state = (object == nullptr);
		if (state)
			delete object;
		return state;
	}
	),m_objects.end());
}

ax::ObjectManager & ax::ObjectManager::getInstance()
{
	static ObjectManager objectManager;
	return objectManager;
}

ax::Object::Object():
	m_destroy(false)
{
	ObjectManager::getInstance().m_objects.push_back(this);
	ax::Logger::log(10, "Object created", "Axios",Logger::INFO,"Object");
}

ax::Object::~Object()
{
	ax::Logger::log(10, "Object destroyed", "Axios", Logger::INFO, "Object");
}
