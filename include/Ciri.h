#pragma once
#include <string>
#include <vector>
#include "INIReader.h"

// Read an INI file using INI Reader but allow to include an argument list to be used in the file.
// Also allows to get vector types for the supported inih types
class Ciri : public INIReader {

	protected:
		// Vector with the input arguments
		const std::vector<std::string> _args;

		// Loaded file
		const std::string _file;

		// Returns the input string but replacing any instance of an argument $(N) with the
		// argument in position N. If there is no argument N does nothing.
		std::string replace_args(const std::string& input) const;

		// Returns the input string but replacing any insatnce of a reference to another section-key
		// pair ${section=key} with the value of said key. If there is no key does nothing.
		std::string replace_refs(const std::string& input) const;

	public:
		// Constructs Ciri and parse given filename, set the Ciri arguments to args
		INI_API explicit Ciri(const std::string& filename, std::vector<std::string> args = {});

		// Constructs Ciri and parse given buffer, set the Ciri arguments to args
		INI_API explicit Ciri(const char* buffer, size_t size, std::vector<std::string> args = {});

		// Get a string value from an INI file, performing argument and reference replacements.
		INI_API std::string Get(const std::string& section, const std::string& name, const std::string& default_value = "") const;

		// Get a vector of the values in an INI file, if the value is not a list return the default value.
		// Supports a normal list {1,2,4} or a repetition list (2,10) -> {10,10}
		INI_API std::vector<std::string> GetList(const std::string& section, const std::string& name, const std::vector<std::string>& default_value) const;

		// Get a list of strings from INI file using {} or repetition syntax, returning default_value if not found or invalid.
		INI_API std::vector<std::string> GetStringList(const std::string& section, const std::string& name, const std::vector<std::string>& default_value) const;

		// Get a list of integers (long) from INI file, returning default_value if not found or not valid integers (decimal "1234", "-1234", or hex "0x4d2").
		INI_API std::vector<long> GetIntegerList(const std::string& section, const std::string& name, const std::vector<long>& default_value) const;

		// Get a list of unsigned integers (unsigned long) from INI file, returning default_value if not found or not valid (decimal or hex).
		INI_API std::vector<unsigned long> GetUnsignedList(const std::string& section, const std::string& name, const std::vector<unsigned long>& default_value) const;

		// Get a list of signed 64-bit integers from INI file, returning default_value if not found or not valid (decimal or hex).
		INI_API std::vector<int64_t> GetInteger64List(const std::string& section, const std::string& name, const std::vector<int64_t>& default_value) const;

		// Get a list of unsigned 64-bit integers from INI file, returning default_value if not found or not valid (decimal or hex).
		INI_API std::vector<uint64_t> GetUnsigned64List(const std::string& section, const std::string& name, const std::vector<uint64_t>& default_value) const;

		// Get a list of real (floating point) values from INI file, returning default_value if not found or not valid.
		INI_API std::vector<double> GetRealList(const std::string& section, const std::string& name, const std::vector<double>& default_value) const;

		// Get a list of boolean values from INI file, returning default_value if not found or not valid (true/false, yes/no, on/off, 1/0).
		INI_API std::vector<bool> GetBooleanList(const std::string& section, const std::string& name, const std::vector<bool>& default_value) const;

		// Add a value to the map, returns 0 on success 1 on error
		INI_API int Add(const std::string& section, const std::string& name, const std::string& value);

		// Removes an entry from the map, returns 0 on success 1 on error
		INI_API int Remove(const std::string& section, const std::string& name);

		// Writes the contents to a file, overwritting if exists. Returns 0 on success 1 on error
		INI_API int Save(const std::string& output) const;

		// Returns the loaded file
		INI_API std::string File() const;


};

