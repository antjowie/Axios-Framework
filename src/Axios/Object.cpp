#include "Axios/Object.h"

#include <algorithm>

ax::ObjectManager::ObjectManager(ax::ObjectFactory &objectFactory):
	m_objectFactory(objectFactory)
{
	m_objects.resize(Object::Type::Count);
}

ax::ObjectManager::~ObjectManager()
{
	for (auto &type : m_objects)
		for (auto &object : type)
			delete object;
}

void ax::ObjectManager::_update(const double elapsedTime)
{
	for (auto &type : m_objects)
		for (auto &object : type)
			object->update(elapsedTime);
}

void ax::ObjectManager::_draw(sf::RenderWindow & window, const double & accumulator)
{
	for (auto &type : m_objects)
		for (auto &object : type)
			window.draw(*object);
}

void ax::ObjectManager::_add(Object * object)
{
	m_objects[object->m_type].push_back(object);
	_m_referenceManager._m_array[object->m_referenceIndex] = object;
}

void ax::ObjectManager::_clear()
{	
	// This deletes all the objects that have their delete flag set to true.
	// Because the memory is deallocated, we can just remove the pointers from the vector
	for(auto &deleter : m_objects)
	deleter.erase(std::remove_if(deleter.begin(),deleter.end(),
		[&](Object *object)
	{
		if (object->m_destroy)
		{
			// Erase object from reference manager
			_m_referenceManager._m_array[object->m_referenceIndex] = nullptr;
			
			// Delete the object
			delete object;

			// Erase object from data container
			return true;
		}
		return false;
	}
	),deleter.end());
}

void ax::ObjectManager::_load(const std::vector<std::unordered_map<std::string, std::string>>& objectsState)
{
	for (const auto &data : objectsState)
		m_objectFactory.construct(data.at("name").c_str(), data);
}

const std::vector<std::unordered_map<std::string, std::string>> ax::ObjectManager::_save() const
{
	std::vector<std::unordered_map<std::string, std::string>> objectsState;

	for (const auto& type : m_objects)
		for (const auto& object : type)
		{
			// Convert map to correct format
			// Note: I should do something about these converters
			// but I'm not sure what
			std::unordered_map<std::string,std::string> map;
			for (const auto& element: object->_m_serializeMap)
				switch (element.second.type)
				{
				case Object::SerializeItem::Type::Int:
					map[element.first] = std::to_string(*static_cast<int*>(element.second.address));
					break;
				case Object::SerializeItem::Type::Float:
					map[element.first] = std::to_string(*static_cast<float*>(element.second.address));
					break;
				case Object::SerializeItem::Type::String:
					map[element.first] = *static_cast<std::string*>(element.second.address);
					break;
				case Object::SerializeItem::Type::Bool:
					map[element.first] = std::to_string(*static_cast<bool*>(element.second.address));
					break;
				}

			objectsState.push_back(map);
		}

	return objectsState;
}

void ax::Object::draw(sf::RenderTarget & window, sf::RenderStates states) const
{
}

void ax::Object::addToSerialization(const char * name, void * address, const SerializeItem::Type type)
{
	_m_serializeMap.emplace(name,SerializeItem{ type,address });
}

void ax::Object::update(const double elapsedTime)
{
	if (m_timeTillDestroy == -1)
		return;

	m_timeTillDestroy -= (float)elapsedTime;
	if (m_timeTillDestroy <= 0)
		m_destroy = true;
}

void ax::Object::destroy(float timeTillDestroy)
{
	m_timeTillDestroy = timeTillDestroy;
}

ax::Object::Object(ObjectFactory &objectFactory, const Type type, const char* name):
	m_objectFactory(objectFactory),
	m_referenceIndex(objectFactory._m_objectManager._m_referenceManager._getFirstFreeIndex()),
	m_uniqueReferenceID(objectFactory._m_objectManager._m_referenceManager._getUniqueID()),
	m_type(type),
	m_name(name),
	m_destroy(false)
{
	ax::Logger::log(10, std::string("Created a " + std::string(m_name) + 
		" with the ID " + std::to_string(m_uniqueReferenceID)).c_str(), "Axios",Logger::INFO,"Object");

	// Used for when the object is being constructed
	addToSerialization("name", &m_name, SerializeItem::Type::String); 
 	addToSerialization("timeTillDestroy", &m_timeTillDestroy, SerializeItem::Type::Float); 
}

ax::Object::~Object()
{
	ax::Logger::log(10, std::string("Destroyed a " + std::string(m_name) + 
		" with the ID " + std::to_string(m_uniqueReferenceID)).c_str(), "Axios", Logger::INFO, "Object");
}

unsigned int ax::ObjectReferenceManager::_getUniqueID()
{
	static unsigned int ID{ 0 };
	return ID++;
}

unsigned int ax::ObjectReferenceManager::_getFirstFreeIndex() const
{
	unsigned int index{ 0 };
	while (index < maxObjectCount)
	{
		if (_m_array[index] == nullptr)
		{
			return index;
		}
		index++;
	}

	ax::Logger::log(0, "Maximum Object count surpassed", "Axios", ax::Logger::ERROR, "ObjectReferenceManager");
	return -1;
}

ax::ObjectReference::ObjectReference(Object * object) :
	m_referenceIndex(object->m_referenceIndex), 
	m_uniqueReferenceID(object->m_uniqueReferenceID),
	m_referenceManager(object->m_objectFactory._m_objectManager._m_referenceManager)
{
}
 
ax::Object * ax::ObjectReference::get()
{
	Object *object = m_referenceManager._m_array[m_referenceIndex];
	if (object != nullptr && object->m_uniqueReferenceID == m_uniqueReferenceID)
		return object;

	return nullptr;
}

ax::ObjectFactory::ObjectFactory(ObjectManager & objectManager):
	_m_objectManager(objectManager)
{
}

ax::Object::SerializeItem & ax::Object::SerializeItem::operator=(const SerializeItem & item)
{
	this->address = item.address;
	this->type = item.type;
	return *this;
}

ax::Object::SerializeItem::SerializeItem(const Type type, void * address):
	type(type),address(address)
{
}