#include "parser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

using IniData = std::unordered_map<std::string, std::unordered_map<std::string, std::string>>;

// Helper to trim leading/trailing spaces
std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \"\t");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \"\t");
    return s.substr(start, end - start + 1);
}

// Core parsing function (throws on error)
void parse_core(const std::string& data, IniData& result) {
    std::istringstream stream(data);
    std::string line;
    std::string current_section;
    size_t line_number = 0;

    while (std::getline(stream, line)) {
        ++line_number;
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;  // empty or comment

        if (line.front() == '[' && line.back() == ']') {  // section
            current_section = trim(line.substr(1, line.size() - 2));
            if (current_section.empty())
                throw std::runtime_error("Empty section name at line " + std::to_string(line_number));
        } else {
            size_t eq_pos = line.find('=');
            if (eq_pos == std::string::npos)
                throw std::runtime_error("Missing '=' in line " + std::to_string(line_number));

            std::string key = trim(line.substr(0, eq_pos));
            std::string value = trim(line.substr(eq_pos + 1));
            if (key.empty())
                throw std::runtime_error("Empty key at line " + std::to_string(line_number));

            // Concatenate if key exists
            result[current_section][key] += value;
        }
    }
}

// Parses the contents of a file
IniData parse_file(const std::string& file_path) {
	std::ifstream file(file_path);
	if (!file.is_open())
		throw std::runtime_error("Cannot open file: " + file_path);

	std::ostringstream buffer;
	buffer << file.rdbuf();
	IniData result;
	parse_core(buffer.str(), result);
	return result;
}

// Parses the contents of a string
IniData parse_string(const std::string& input) {
	IniData result;
	parse_core(input, result);
	return result;
}

// Save the contents of data into file_path
void save_ini(const IniData& data, const std::filesystem::path& file_path) {
    std::ofstream out(file_path);
    if (!out) throw std::runtime_error("Failed to open file for writing: " + file_path.string());

    for (const auto& [section, keys] : data) {
        out << "[" << section << "]\n";
        for (const auto& [key, value] : keys) {
            out << key << " = " << value << "\n";
        }
        out << "\n";
    }
}

