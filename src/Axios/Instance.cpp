#include "Axios/Instance.h"
#include "Axios/InputHandler.h"
#include "Axios/DataManager.h"


#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <list>
#include <fstream>
#include <sstream>
#include <iostream>

float getAverage(const std::list<float> & list)
{
	float total{ 0 };
	for (const auto &ms : list)
		total += ms;
	return total / list.size();
}

void ax::Instance::start()
{
    std::list<float> averageTime; // A class that keeps track of the time elapsed between reset calls
    sf::Clock frameTime; 
    sf::RenderWindow window(sf::VideoMode(std::stoi(ax::DataManager::Config().data["windowX"]), std::stoi(ax::DataManager::Config().data["windowY"])),ax::DataManager::Config().data["title"]);
	float refreshRate;
	sf::Event event;
    float elapsedTime;
    elapsedTime = frameTime.restart().asSeconds();

    while(window.isOpen())
    {
        // If the game has a fps lower than 0, skip the frame
        if (elapsedTime > 1)
        {
            elapsedTime = 0;
            continue;
        }

		// If user were to change the refreshRate, If std::stof is to expansive
		// write a struct with int and string and convert during loading
		refreshRate = std::stof(ax::DataManager::Config().data["refreshRate"]);
        if (elapsedTime >= 1.f / refreshRate)
        {
        ax::InputHandler::getInstance().update(window,event);
            averageTime.push_front(elapsedTime);
			if (averageTime.size() > 25) // Max elements
				averageTime.pop_back();

            elapsedTime = std::fmod(elapsedTime, 1.f / refreshRate);
			
			window.clear();

            //updateScene(averageTime.getAverage());
			
			window.display();
        }
        elapsedTime += frameTime.restart().asSeconds();
    }
}

void ax::Instance::init()
{
	ax::DataManager::Config().load("config/config.json");
	ax::DataManager::Config().check("windowX", "1280");
	ax::DataManager::Config().check("windowY", "720");
	ax::DataManager::Config().check("refreshRate", "144");
	ax::DataManager::Config().check("title", "Axios Framework");
	
	ax::DataManager::GameKey().load("config/keybindings.json");
	ax::DataManager::GameKey().check("up", { sf::Keyboard::Space, -1 });
	ax::DataManager::GameKey().check("left", { sf::Keyboard::A, -1 });
	ax::DataManager::GameKey().check("right", { sf::Keyboard::D, -1 });
	// Init ax::InputHandler with correct data
}

void ax::Instance::deinit()
{
	ax::DataManager::Config().save("config/config.json");
	ax::DataManager::GameKey().save("config/keybindings.json");
}
