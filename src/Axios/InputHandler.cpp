#include "Axios/InputHandler.h"

#include <SFML/Window/Event.hpp>

ax::InputHandler& ax::InputHandler::getInstance()
{
	static ax::InputHandler instance;
	return instance;
}

void ax::InputHandler::update(sf::RenderWindow & window, sf::Event &event)
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

bool ax::InputHandler::isKeyPressed(const int keyName) const
{
	return m_keys[keyName].isPressed;
}

bool ax::InputHandler::isKeyHold(const int keyName) const
{
	return m_keys[keyName].isHold;
}

bool ax::InputHandler::isKeyReleased(const int keyName) const
{
	return m_keys[keyName].isReleased;
}

bool ax::InputHandler::isButtonPressed(const int buttonName) const
{
	return m_buttons[buttonName].isPressed;
}

bool ax::InputHandler::isButtonHold(const int buttonName) const
{
	return m_buttons[buttonName].isHold;
}

bool ax::InputHandler::isButtonReleased(const int buttonName) const
{
	return m_buttons[buttonName].isReleased;
}

bool ax::InputHandler::isItemPressed(const KeyItem &item) const
{
	if (item[0] != -1)
		return isKeyPressed(static_cast<sf::Keyboard::Key>(item[0]));
	else if (item[1] != -1)
		return isButtonPressed(static_cast<sf::Mouse::Button>(item[1]));
	return false;
}

bool ax::InputHandler::isItemHold(const KeyItem &item) const
{
	if (item[0] != -1)
		return isKeyHold(static_cast<sf::Keyboard::Key>(item[0]));
	else if (item[1] != -1)
		return isButtonHold(static_cast<sf::Mouse::Button>(item[1]));
	return false;
}

bool ax::InputHandler::isItemReleased(const KeyItem &item) const
{
	if (item[0] != -1)
		return isKeyReleased(static_cast<sf::Keyboard::Key>(item[0]));
	else if (item[1] != -1)
		return isButtonReleased(static_cast<sf::Mouse::Button>(item[1]));
	return false;
}

ax::InputHandler::InputHandler()
{
	m_keys.resize(sf::Keyboard::KeyCount);
	m_buttons.resize(sf::Mouse::ButtonCount);
}

