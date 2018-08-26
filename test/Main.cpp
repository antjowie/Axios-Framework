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
	int m_damage;
public:
	TempObject(ax::ObjectFactory & objectFactory):
		Object(objectFactory,Type::Hud,"temp")
	{
		addToSerialization("attack", &m_attack, SerializeItem::Type::String);
		addToSerialization("damage", &m_damage, SerializeItem::Type::Int);
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