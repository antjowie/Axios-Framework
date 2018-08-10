#include "Axios/InputHandler.h"
#include "Axios/Logger.h"

#include <SFML/Window/Event.hpp>

ax::InputHandler& ax::InputHandler::getInstance()
{
	static ax::InputHandler instance;
	return instance;
}

void ax::InputHandler::_update(sf::RenderWindow & window, sf::Event &event)
{
	m_anyKeyPressed = false;

	for (auto &iter : m_keys)
	{
		iter.isPressed = false;
		iter.isReleased = false;
	}
	for (auto &iter : m_buttons)
	{
		iter.isPressed = false;
		iter.isReleased = false;
	}

	while (window.pollEvent(event))
	{
		if (!window.hasFocus()) continue;
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			m_anyKeyPressed = true;

			if (!m_keys[event.key.code].isHold)
				m_keys[event.key.code].isPressed = true;
			m_keys[event.key.code].isHold = true;
			break;

		case sf::Event::KeyReleased:
			m_keys[event.key.code].isHold = false;
			m_keys[event.key.code].isReleased = true;
			break;

		case sf::Event::MouseButtonPressed:
			m_anyKeyPressed = true;

			if (!m_buttons[event.key.code].isHold)
				m_buttons[event.key.code].isPressed = true;
			m_buttons[event.key.code].isHold = true;
			break;

		case sf::Event::MouseButtonReleased:
			m_buttons[event.key.code].isHold = false;
			m_buttons[event.key.code].isReleased = true;
			break;

		case sf::Event::Closed:
			window.close();
			break;
		}
	}
}

bool ax::InputHandler::isAnyKeyPressed() const
{
	return m_anyKeyPressed;
}

const ax::InputHandler::KeyState& ax::InputHandler::getKey(const int keyName) const
{
	return m_keys[keyName];
}

const ax::InputHandler::KeyState& ax::InputHandler::getButton(const int buttonName) const
{
	return m_buttons[buttonName];
}
const ax::InputHandler::KeyState ax::InputHandler::getItem(const KeyItem &item, const char* name) const
{
	if (item[0] != item[1])
	{
		if (item[0] != -1)
			return getKey(item[0]);
		else if (item[1] != -1)
			return getKey(item[1]);
	}
	Logger::log(5, std::string("item " + std::string(name) + " doesn't exist").c_str(), "Axios", Logger::WARNING, "InputHandler");
	return ax::InputHandler::KeyState();
}

const ax::InputHandler::KeyState ax::InputHandler::getItem(const char* name) const
{
	return getItem(ax::DataManager::GameKey().data[name],name);
}

ax::InputHandler::InputHandler()
{
}