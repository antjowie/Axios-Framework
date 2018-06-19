#pragma once
#include <nlohmann/json.hpp>

#include <array>
#include <fstream>
#include <unordered_map>

// index 0 = keyboard, 1 = mouse
using KeyItem = std::array<int, 2>;

namespace ax
{	
	class DataManager
	{
	private:
		using callbackFunction = void(*)(const bool force);
		using json = nlohmann::json;
		
		template <class T>
		class Data
		{
		private:
			callbackFunction m_extraCheck;
	
		public:
			std::unordered_map<std::string, T> data;
			
			// Used for the callback function
			void check(const char* name, const T &value, const bool force = false);
			// Used to check whole file
			void check(const bool force = false);
			void load(const char* path);
			void save(const char* path);
		
			Data(const callbackFunction &extraCheck);
		};
		
	public:
		static Data<std::string>& Config(const callbackFunction &extraCheck = nullptr);
		static Data<KeyItem>& GameKey(const callbackFunction &extraCheck = nullptr);
	};

	template<class T>
	inline void DataManager::Data<T>::check(const char * name, const T &value, const bool force)
	{
		if (force || data.count(name) == 0)
			data[name] = value;
	}

	template<class T>
	inline void DataManager::Data<T>::check(const bool force)
	{
		m_extraCheck(force);
	}

	template<class T>
	inline void DataManager::Data<T>::load(const char * path)
	{
		std::ifstream configFile(path);
		json config;
		if (configFile.is_open())
		{
			std::stringstream stream;
			stream << configFile.rdbuf();
			config = json::parse(stream.str());
		}

		for (json::iterator it = config.begin(); it != config.end(); ++it)
			data[it.key()] = it.value().get<T>();
	}
	template<class T>
	inline void DataManager::Data<T>::save(const char * path)
	{
		std::ofstream configFile(path, std::ios::trunc);
		json config(data);
		configFile << config.dump();
		configFile.close();
	}

	template<class T>
	inline DataManager::Data<T>::Data(const callbackFunction & extraCheck):
		m_extraCheck(extraCheck)
	{
	}
}