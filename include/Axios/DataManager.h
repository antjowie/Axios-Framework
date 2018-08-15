#pragma once
#include <nlohmann/json.hpp>

#include <fstream>
#include <unordered_map>

namespace ax
{	
	class DataManager
	{
	public:
		// This is a default config layout
		struct Config
		{
			const char* category;
			const char* key;
			const char* value;

			Config(const char* category, const char* key, const char* value);
		};

	private:
		using json = nlohmann::json;
		
		// The first map represents the category
		// The second map represents the configuration
		std::unordered_map<std::string, 
			std::unordered_map<std::string, std::string>> m_data;
		
		std::vector<Config> m_control;

	public:
		// Add an config to the default configuration.
		// Call this function if you want to save
		// the state of any configuration
		// 
		// If a config occurs twice, it is up to the bool to 
		// overwrite it or not.
		void addToDefaultConfig(const Config &config, const bool overwriteOnOccurance);

		// Gets the value of a config
		// If the config is not found, returns -1
		std::string getConfig(const char* category, const char* key) const;

		// This sets a value into the config
		// If the config already exists, overwrite it.
		// If it doesn't, add it to the file.
		// Only the configurations inside of the control
		// vector will be saved. Be sure to call the function
		// addToDefaultConfig if the configurations state has
		// to be saved
		void setConfig(const Config& config);

		// Import data from path
		void _load(const char* path);

		// Export data to path
		void _save(const char* path);

		// Check the config with configs inside the control vector
		// If the name inside the data doesn't occur inside of the 
		// control vector, it is removed.
		//
		// If overwriteToDefault is true, all the settings will be
		// set to the default state according to the control vector
		void _checkConfig(bool overwriteToDefault);

		static DataManager& getInstance();
	};
}