#include "Axios/Instance.h"
#include "Axios/InputHandler.h"
#include "Axios/DataManager.h"
#include "Axios/Logger.h"
#include "Axios/Object.h"

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <sstream>

unsigned int ax::Instance::UpdateLoop::m_id = 0;

float getAverage(const std::list<float> & list)
{
	float total{ 0 };
	for (const auto &ms : list)
		total += ms;
	return total / list.size();
}

ax::Instance::Instance():
	m_init(false)
{
}

void ax::Instance::updateUserConfig()
{
	m_window.create(sf::VideoMode(std::stoi(ax::DataManager::Config().data["windowX"]), std::stoi(ax::DataManager::Config().data["windowY"])), 
		m_currentTitle, ((ax::DataManager::Config().data["fullscreen"] == "1") ? sf::Style::Fullscreen : sf::Style::Default));
	m_refreshRate = std::stof(ax::DataManager::Config().data["refreshRate"]);
}

void ax::Instance::setTitle(const char * title, const bool & append)
{
	if (!append)
		m_currentTitle = title;
	else
		m_currentTitle = ax::DataManager::Config().data["title"] + " | " + std::string(title);

	if (m_window.isOpen())
		m_window.setTitle(m_currentTitle);
}

void ax::Instance::init()
{
	// Load config data
	ax::DataManager::Config()._load("config/config.json");
	ax::DataManager::GameKey()._load("config/keybindings.json");

	// Check framework related config
	ax::DataManager::Config().check("title", "No title");
	ax::DataManager::Config().check("verbosity", "10");

	// Check user related config
	ax::DataManager::Config().check("windowX", std::to_string(sf::VideoMode().getDesktopMode().width));
	ax::DataManager::Config().check("windowY", std::to_string(sf::VideoMode().getDesktopMode().height));
	ax::DataManager::Config().check("fullscreen", "0");
	ax::DataManager::Config().check("refreshRate", "144");

	ax::DataManager::GameKey()._check(false);

	setTitle(ax::DataManager::Config().data["title"].c_str(), false);
	updateUserConfig();

	m_init = true;
}

void ax::Instance::start()
{
	struct Temp : public ax::Object, public ax::UpdateLoopObject
	{
		const char* name;

	virtual void update(const float elapsedTime) override final
	{
		ax::Logger::log(0, name, "Update");
	}

		Temp(const char* name)
		:ax::UpdateLoopObject(ax::UpdateLoopType::Real), name(name)
		{
			ax::Logger::log(0, name, "Created");
		}

		~Temp()
		{
			ax::Logger::log(0, name, "Destroyed");
		}
	};

	std::vector<Temp*> t;
	t.push_back(new Temp("0"));
	t.push_back(new Temp("0"));
	t.push_back(new Temp("0"));
	
	// Check if user called init function
	if (!m_init)
	{
		ax::Logger::log(5, "Init hasn't been called before start, custom configurations will not be applied", "Axios", ax::Logger::WARNING, "Instance");
		init();
	}

	// These values can't be changed by the user during runtime, that's why they are local to this scope
    std::list<float> averageTime; // A class that keeps track of the time elapsed between reset calls
    sf::Clock frameTime; 
	sf::Event event;
    float elapsedTime;
    elapsedTime = frameTime.restart().asSeconds();

	// -----
	// Loop phase
    while(m_window.isOpen())
    {
        // If the game has a fps lower than 0, skip the frame
        if (elapsedTime > 1)
        {
            elapsedTime = 0;
            continue;
        }

		if (elapsedTime >= 1.f / m_refreshRate)
        {
			ax::InputHandler::getInstance()._update(m_window,event);   
			averageTime.push_front(elapsedTime);
			if (averageTime.size() > 5) // Max elements
				averageTime.pop_back();

            elapsedTime = std::fmod(elapsedTime, 1.f / m_refreshRate);
			
			m_window.clear();

			//m_realUpdateLoop._update(averageTime.getAverage());
			m_physicsUpdateLoop._update(elapsedTime);
			m_realUpdateLoop._update(elapsedTime);

			if (!t.empty())
			{
				t.back()->m_destroy = true;
				t.pop_back();
			}

			ax::ObjectManager::getInstance()._clear();
			m_physicsUpdateLoop._clear();
			m_realUpdateLoop._clear();
			
			m_window.display();
        }
        elapsedTime += frameTime.restart().asSeconds();
    }

	// -----
	// Deinit phase
	ax::DataManager::Config()._save("config/config.json");
	ax::DataManager::GameKey()._save("config/keybindings.json");
}

ax::Instance & ax::Instance::getInstance()
{
	static Instance instance;
	return instance;
}

void ax::UpdateLoopObject::update(const float elapsedTime)
{
}

void ax::UpdateLoopObject::physicsUpdate(const float elapedTime)
{
}

ax::UpdateLoopObject::UpdateLoopObject():
	m_id(Instance::UpdateLoop::_getId())
{
}

ax::UpdateLoopObject::~UpdateLoopObject()
{
	if(m_hookedToUpdate)
		Instance::getInstance().m_realUpdateLoop.m_destroyIds.push_back(m_id);
	if(m_hookedToPhysicsUpdate)
		Instance::getInstance().m_physicsUpdateLoop.m_destroyIds.push_back(m_id);
}

void ax::Instance::UpdateLoop::_update(const float elapsedTime)
{
	m_elapsedTime += elapsedTime;
	
	// Checks if objects should be updated
	if (m_interval != 0 && m_elapsedTime == std::fmod(m_elapsedTime, m_interval))
		return;
	
	const float modulo = (m_interval == 0) ? m_elapsedTime : m_interval;
	
	do
	{
		for (auto iter : m_updateLoop)
			iter->update(modulo);

		m_elapsedTime -= modulo;
	} while (m_elapsedTime > modulo);
}

void ax::Instance::UpdateLoop::_clear()
{
	// Pretty inefficient code, but we will see if this code will be a bottleneck 
	// in the future.
	m_updateLoop.erase(std::remove_if(m_updateLoop.begin(),m_updateLoop.end(),
		[&](ax::UpdateLoopObject *object)
		{
			bool hit{ false };
			for (const auto &iter : m_destroyIds)
				if (object->m_id == iter)
					hit = true;
			return hit;
		}
	),m_updateLoop.end());

	m_destroyIds.clear();
}

unsigned int ax::Instance::UpdateLoop::_getId()
{
	return m_id++;
}

ax::Instance::UpdateLoop::UpdateLoop(const float interval):
	m_interval(interval),m_elapsedTime(0),m_id(0)
{
}