#pragma once

#include <string>
#include <unordered_map>
#include <filesystem>

using IniData = std::unordered_map<std::string, std::unordered_map<std::string, std::string>>;

// Parses the contents of a file and returns the parsed data.
// Throws std::runtime_error on error.
IniData parse_file(const std::string& file_path);

// Parses the contents of a string and returns the parsed data.
// Throws std::runtime_error on error.
IniData parse_string(const std::string& input);

// Core parsing function (internal, can throw std::runtime_error on error)
void parse_core(const std::string& data, IniData& result);

// Save IniData to file
void save_ini(const IniData& data, const std::filesystem::path& file_path);

