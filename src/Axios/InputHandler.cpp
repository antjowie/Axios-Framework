#include "Axios/InputHandler.h"
#include "Axios/Logger.h"

#include <SFML/Window/Event.hpp>

void ax::InputHandler::setState(const KeyItem & keyItem)
{
	std::string keyCode;
	switch (keyItem.keyType)
	{
	case KeyType::Keyboard:
		keyCode.push_back('k');
		break;
	
	case KeyType::Mouse:
		keyCode.push_back('m');
		break;
	}
	keyCode.append(std::to_string(keyItem.key));
	ax::DataManager::getInstance().setConfig({ "keybinding",keyItem.name,keyCode.c_str() });
}

ax::InputHandler& ax::InputHandler::getInstance()
{
	static ax::InputHandler instance;
	return instance;
}

void ax::InputHandler::_update(sf::RenderWindow & window, sf::Event &event)
{
	m_anyKeyPressed = false;

	for (int index = 0; index < KeyType::Count; index++)
		for (auto &iter : m_keyItems[index])
		{
			iter.isPressed = false;
			iter.isReleased = false;
		}

	while (window.pollEvent(event))
	{
		if (!window.hasFocus()) 
			continue;
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			m_anyKeyPressed = true;

			if (!m_keyItems[KeyType::Keyboard][event.key.code].isHold)
				m_keyItems[KeyType::Keyboard][event.key.code].isPressed = true;
			m_keyItems[KeyType::Keyboard][event.key.code].isHold = true;
			break;

		case sf::Event::KeyReleased:
			m_keyItems[KeyType::Keyboard][event.key.code].isHold = false;
			m_keyItems[KeyType::Keyboard][event.key.code].isReleased = true;
			break;

		case sf::Event::MouseButtonPressed:
			m_anyKeyPressed = true;

			if (!m_keyItems[KeyType::Mouse][event.key.code].isHold)
				m_keyItems[KeyType::Mouse][event.key.code].isPressed = true;
			m_keyItems[KeyType::Mouse][event.key.code].isHold = true;
			break;

		case sf::Event::MouseButtonReleased:
			m_keyItems[KeyType::Mouse][event.key.code].isHold = false;
			m_keyItems[KeyType::Mouse][event.key.code].isReleased = true;
			break;

		case sf::Event::Closed:
			window.close();
			break;
		}
	}
}

const bool ax::InputHandler::isAnyKeyPressed() const
{
	return m_anyKeyPressed;
}

const bool ax::InputHandler::isAnyControllerConnected() const
{
	return m_controllerDetected;
}

const ax::InputHandler::KeyState ax::InputHandler::getState(const char* name) const
{
	std::string key{ ax::DataManager::getInstance().getConfig("keybinding",name) };
	if (key == "-1")
	{
		Logger::log(5, std::string("item " + std::string(name) + " doesn't exist").c_str(), "Axios", Logger::WARNING, "InputHandler");
		return ax::InputHandler::KeyState();
	}

	KeyType type;
	if (key[0] == 'm')
		type = KeyType::Mouse;
	else if (key[1] == 'k')
		type = KeyType::Keyboard;

	key.erase(0);
	return getState(KeyItem(name, type, std::stoi(key)));
}

const ax::InputHandler::KeyState ax::InputHandler::getState(const KeyItem & state) const
{
	return m_keyItems[state.keyType][state.key];
}

ax::InputHandler::InputHandler():
	m_anyKeyPressed(false),m_controllerDetected(sf::Joystick::isConnected(0))
{
	m_keyItems[Keyboard].resize(sf::Keyboard::KeyCount);
	m_keyItems[Mouse].resize(sf::Mouse::ButtonCount);
}

ax::InputHandler::KeyItem::KeyItem(const char * name, KeyType keyType, int key):
	name(name),keyType(keyType),key(key)
{
}