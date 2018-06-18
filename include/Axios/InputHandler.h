#pragma once
#include "DataManager.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <vector>

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
		std::vector<KeyState> m_keys;
		std::vector<KeyState> m_buttons;
		bool m_anyKeyPressed;

		InputHandler();

	public:
		// Uses an event from the game loop to not skip events
		void update(sf::RenderWindow & window, sf::Event &event);

		bool isAnyKeyPressed() const;

		// Used to check individual key
		bool isKeyPressed(const int keyName) const;
		// Used to check individual key						  
		bool isKeyHold(const int keyName) const;
		// Used to check individual key						  
		bool isKeyReleased(const int keyName) const;

		// Used to check individual button
		bool isButtonPressed(const int buttonName) const;
		// Used to check individual button							 
		bool isButtonHold(const int buttonName) const;
		// Used to check individual button							 
		bool isButtonReleased(const int buttonName) const;

		// Used to check item from config
		bool isItemPressed(const KeyItem &item) const;
		// Used to check item from config
		bool isItemHold(const KeyItem &item) const;
		// Used to check item from config
		bool isItemReleased(const KeyItem &item) const;

		static InputHandler &getInstance();
	};

	namespace InputName
	{
		const std::string KeyboardToString(const int key);
		const std::string MouseToString(const int button);
	}
}