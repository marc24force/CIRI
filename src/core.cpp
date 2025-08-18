#include "ciri.h"
#include <sstream>
#include <cctype>
#include <stdexcept>
#include <regex>
#include "parser.h"

Ciri::Ciri(const std::filesystem::path& file_path, const std::vector<std::string> args) : _args(args) {
	_data = parse_file(file_path.string());
	_dir = std::filesystem::weakly_canonical(file_path.parent_path());
}

Ciri::Ciri(const std::string& content, const std::vector<std::string> args) : _args(args) {
	_data = parse_string(content);
	_dir = std::filesystem::current_path();
}

std::filesystem::path Ciri::directory() {
	return _dir;
}

void Ciri::save(const std::filesystem::path& file_path) const {
	save_ini(_data, file_path);
}

bool Ciri::exists(const std::string& section) const {
	return _data.find(section) != _data.end();
}

bool Ciri::exists(const std::string& section, const std::string& key) const {
	auto it = _data.find(section);
	if (it == _data.end()) return false;
	return it->second.find(key) != it->second.end();
}

std::vector<std::string> Ciri::sections() const {
	std::vector<std::string> result;
	for (const auto& [section, _] : _data) {
		result.push_back(section);
	}
	return result;
}

std::vector<std::string> Ciri::keys(const std::string& section) const {
	std::vector<std::string> result;
	auto it = _data.find(section);
	if (it == _data.end()) return result;
	for (const auto& [key, _] : it->second) {
		result.push_back(key);
	}
	return result;
}

void Ciri::add(const std::string& section, const std::string& key, const std::string& value) {
	_data[section][key] += value;
}

void Ciri::remove(const std::string& section, const std::string& key) {
	auto sec_it = _data.find(section);
	if (sec_it != _data.end()) {
		sec_it->second.erase(key);
		if (sec_it->second.empty())
			_data.erase(sec_it);
	}
}


std::string Ciri::_get(const std::string& section, const std::string& key) const {
	auto sec_it = _data.find(section);
	auto key_it = sec_it->second.find(key);
	std::string value = key_it->second;
	value = _replace_envs(value);
	value = _replace_args(value);
	value = _replace_cmds(value);
	value = _replace_refs(value);
	return value;
}

std::vector<std::string> flatten_list(const std::string &s) {
	std::vector<std::string> result;
	std::string token;
	char prev = 0;

	auto trim = [](std::string &str) {
		str.erase(0, str.find_first_not_of(" \t"));
		str.erase(str.find_last_not_of(" \t") + 1);
	};

	for (char c : s) {
		if (c == '{' || c == '}' || c == ',') {
			trim(token);
			if (!token.empty() || ((prev == ',' || prev == '{') && c == ',') || (prev == ',' && c == '}'))
				result.push_back(token);
			token.clear();
			prev = c;
		} else {
			token += c;
			prev = c;
		}
	}

	trim(token);
	if (!token.empty()) result.push_back(token);

	return result;
}

std::vector<std::string> Ciri::_getList(const std::string& section, const std::string& key) const {
	if (!exists(section, key)) return {};
	std::string value = _get(section, key);

	std::smatch match;
	std::regex repeat_pattern(R"(\(\s*(\d+)\s*,\s*([^\)]+)\s*\))");

	while (std::regex_search(value, match, repeat_pattern)) {
		int count = std::stoi(match[1].str());
		std::string element = match[2].str();

		std::ostringstream oss;
		oss << "{";
		for (int i = 0; i < count; ++i) {
			if (i > 0) oss << ",";
			oss << element;
		}
		oss << "}";

		value.replace(match.position(0), match.length(0), oss.str());
	}

	 return flatten_list(value);
}
