#pragma once
#include "DataManager.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <array>

namespace ax
{
	class InputHandler
	{
	private:
		struct KeyState
		{
			bool isPressed{ false };
			bool isHold{ false };
			bool isReleased{ false };
		};

		// These vectors are filled with all the keys and their state
		std::array<KeyState, sf::Keyboard::KeyCount> m_keys;
		std::array<KeyState, sf::Mouse::ButtonCount> m_buttons;
		bool m_anyKeyPressed;

		InputHandler();

	public:
		// Uses an event from the game loop to not skip events
		void _update(sf::RenderWindow & window, sf::Event &event);

		bool isAnyKeyPressed() const;

		// Used to check individual key
		const KeyState &getKey(const int keyName) const;
		
		// Used to check individual button
		const KeyState &getButton(const int buttonName) const;
		
		// Used to check item from config
		const KeyState getItem(const KeyItem &item, const char* name = "NO_NAME") const;
		// Calls the dataManager internally
		const KeyState getItem(const char* name) const;

		static InputHandler &getInstance();
	};

	// This namespace contains functions to map SFML enumerators to strings
	namespace InputName
	{
		const char* keyToString(const int key);
		const char* buttonToString(const int button);
	}
}