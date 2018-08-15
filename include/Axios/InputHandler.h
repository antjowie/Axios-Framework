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
		enum KeyType
		{
			Keyboard,
			Mouse,
			Count
		};

		struct KeyState
		{
			bool isPressed{ false };
			bool isHold{ false };
			bool isReleased{ false };
		};

		// This array contains all the buttons
		std::array<std::vector<KeyState>, KeyType::Count> m_keyItems;

		bool m_anyKeyPressed;
		bool m_controllerDetected;

		InputHandler();

	public:

		struct KeyItem
		{
			const char* name;
			KeyType keyType;
			int key;

			KeyItem(const char *name, KeyType keyType, int key);
		};

		// Uses an event from the game loop to not skip events
		void _update(sf::RenderWindow & window, sf::Event &event);

		const bool isAnyKeyPressed() const;

		// Item is the name that is given to the keybinding
		const KeyState getState(const char* item) const;

		// Give a state and return the information
		const KeyState getState(const KeyItem &state) const;
		
		static InputHandler &getInstance();
	};

	// This namespace contains functions to map SFML enumerators to strings
	namespace InputName
	{
		const char* keyToString(const int key);
		const char* buttonToString(const int button);
	}
}