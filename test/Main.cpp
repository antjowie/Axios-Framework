#include "Axios/Instance.h"
#include "Axios/DataManager.h"

#include "Axios/Scene.h"
#include "Axios/Object.h"

#include <SFML/Window/Keyboard.hpp>

#include <iostream>

class TempObject : public ax::Object
{
public:
	virtual void update(const double elapsedTime) override final
	{
		ax::Logger::log(9, std::to_string(m_uniqueReferenceID).c_str());
	}

	TempObject(ax::ObjectFactory & objectFactory):
		Object(objectFactory,Type::Hud)
	{
	}
};

class TempScene : public ax::Scene
{
public:
	virtual void onEnter() override final
	{
		m_objectFactory.add<TempObject>("temp");
		m_objectFactory.construct("temp");
		m_objectFactory.construct("temp");
		m_objectFactory.construct("temp");
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