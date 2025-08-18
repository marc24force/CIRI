#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <stdexcept>
#include <filesystem>

class Ciri {
	private:
		using IniData = std::unordered_map<std::string, std::unordered_map<std::string, std::string>>;

		// Map containing the data
		IniData _data;

		// Directory where the ini is located or current_path if loaded from string
		std::filesystem::path _dir;

		// List of input arguments
		const std::vector<std::string> _args;

		// Returns the raw string for a key, or nullptr if not found
		std::string _get(const std::string& section, const std::string& key) const;

		// Returns a vector of strings for a key, separated by comas
		std::vector<std::string> _getList(const std::string& section, const std::string& key) const;

		// Returns the input string but replacing any instance of an argument $N or $(N:(default)?)
		// with the argument in position N if exists, else uses the default. If neither does nothing.
		std::string _replace_args(const std::string& input) const;

		// Returns the input string but replacing any instance of a reference to another section-key
		// pair ${section#key} with the value of said key. If there is no key does nothing.
		std::string _replace_refs(const std::string& input) const;
		
		// Returns the input string but replacing any instance of a environment variable $VAR or 
		// ${VAR:(DEFAULT)?} with the variable value. If the variable doesn't exist uses the default.
		// If DEFAULT is not a variable treats it as text. If no default nor VAR is a variable does nothing.
		std::string _replace_envs(const std::string& input) const;

		// Returns the input string but replacing any instance of a command $[cmd] to the output of said
		// command. If error, returns the error message.
		std::string _replace_cmds(const std::string& input) const;

	public:
		// Construct from file path
		explicit Ciri(const std::filesystem::path& file_path, const std::vector<std::string> args = {});

		// Construct from string
		explicit Ciri(const std::string& content, const std::vector<std::string> args = {});

		// Returns the file directory if loaded from file, else current_path
		std::filesystem::path directory();

		// Stores the current configuration to a file
		void save(const std::string& filename) const;

		// Stores the current configuration to a file
		void save(const std::filesystem::path& file_path) const;

		// Adds or updates a key in a section
		void add(const std::string& section, const std::string& key, const std::string& value);

		// Removes a key from a section. Removes the section if it becomes empty
		void remove(const std::string& section, const std::string& key);
		
		// Check if section exists
		bool exists(const std::string& section) const;

		// Check if key exists in section
		bool exists(const std::string& section, const std::string& key) const;

		// Get a list of the existing sections
		std::vector<std::string> sections() const;

		// Get a list of the existing keys in section
		std::vector<std::string> keys(const std::string& section) const;


		// Template getter
		template <typename T>
		T get(const std::string& section, const std::string& key, const T& default_value, const std::function<T(const std::string&)>& converter) const {
			if (!exists(section, key)) return default_value;
			return converter(_get(section, key));
		}

		// List template getter
		template<class T, class F>
		std::vector<T> getList(const std::string& section, const std::string& key, const std::vector<T>& default_value, F converter) const {
			if (!exists(section, key)) return default_value;

			std::vector<std::string> str_list = _getList(section, key);
			std::vector<T> result;
			result.reserve(str_list.size());

			for (const auto& s : str_list) result.push_back(converter(s));

			return result;
		}


		/* Type-specific Getters */
		std::string getString(const std::string& section, const std::string& key, const std::string& default_value = "") const;
		int getInteger(const std::string& section, const std::string& key, int default_value = 0) const;
		unsigned getUnsigned(const std::string& section, const std::string& key, unsigned default_value = 0) const;
		bool getBoolean(const std::string& section, const std::string& key, bool default_value = false) const;
		float getFloat(const std::string& section, const std::string& key, float default_value = 0.0f) const;
		std::vector<std::string> getStringList(const std::string& section, const std::string& key, const std::vector<std::string>& default_value = {}) const;
		std::vector<int> getIntegerList(const std::string& section, const std::string& key, const std::vector<int>& default_value = {}) const;
		std::vector<unsigned> getUnsignedList(const std::string& section, const std::string& key, const std::vector<unsigned>& default_value = {}) const;
		std::vector<bool> getBooleanList(const std::string& section, const std::string& key, const std::vector<bool>& default_value = {}) const;
		std::vector<float> getFloatList(const std::string& section, const std::string& key, const std::vector<float>& default_value = {}) const;

};

