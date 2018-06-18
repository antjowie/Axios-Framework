#pragma once
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <unordered_map>
#include <array>
// index 0 = keyboard, 1 = mouse
using KeyItem = std::array<int, 2>;

namespace ax
{
	class DataManager
	{
	private:
		template <class T>
		struct Data
		{
			std::unordered_map<std::string, T> data;
			
			void check(const char* name, const T &value);
			void load(const char* path);
			void save(const char* path);
		};
		
	public:
		static Data<std::string>& Config();
		static Data<KeyItem>& GameKey();
	};

	template<class T>
	inline void DataManager::Data<T>::check(const char * name, const T &value)
	{
		if (data.count(name) == 0)
			data[name] = value;
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
}