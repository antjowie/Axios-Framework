#include "Axios/Instance.h"
#include "Axios/DataManager.h"

#include "Axios/Scene.h"
#include "Axios/Object.h"

#include <SFML/Window/Keyboard.hpp>

#include <iostream>

class TempObject : public ax::Object
{
private:
	std::string m_attack;

public:
	TempObject(ax::ObjectFactory & objectFactory):
		Object(objectFactory,Type::Hud,"temp")
	{
		addToSerialization("attack", &m_attack, SerializeItem::Type::String);
	}

	~TempObject()
	{
		ax::Logger::log(0, m_attack.c_str());
	}
};

class TempScene : public ax::Scene
{
public:
	virtual void onEnter() override final
	{
		m_objectFactory.add<TempObject>("temp");
		std::unordered_map<std::string, std::string> map;
		map["attack"] = "I am dashing";
		m_objectFactory.construct("temp",map).get()->destroy(5);
		map["attack"] = "I breath fire";
		m_objectFactory.construct("temp",map).get()->destroy(10);
		map["attack"] = "I explode!!!";
		m_objectFactory.construct("temp",map).get()->destroy(15);
		
		Scene::onEnter();
	}

	TempScene(sf::RenderWindow &window, const char* filePath) :
		Scene(window, filePath)
	{
	}
};

int main()
{
	ax::DataManager::getInstance().addToDefaultConfig(ax::DataManager::Config("keybinding", "left" , "k71"),false);
	ax::DataManager::getInstance().addToDefaultConfig(ax::DataManager::Config("keybinding", "right", "k72"),false);
	ax::DataManager::getInstance().addToDefaultConfig(ax::DataManager::Config("keybinding", "jump" , "k57"),false);

	ax::Instance gameInstance;

	gameInstance.init<TempScene>();

	gameInstance.start();
	
	return 0;
}