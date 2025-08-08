#include "Ciri.h"
#include <iostream>
#include <fstream>
#include <regex>

Ciri::Ciri(const std::string& filename, std::vector<std::string> args)
	: INIReader(filename), _args(std::move(args)), _file(filename) {}

Ciri::Ciri(const char* buffer, size_t size, std::vector<std::string> args)
	: INIReader(buffer, size), _args(std::move(args)) {}

	static std::string trim(const std::string& s) {
		const char* ws = " \t\n\r\f\v";
		size_t start = s.find_first_not_of(ws);
		size_t end = s.find_last_not_of(ws);
		if (start == std::string::npos) return "";

		std::string trimmed = s.substr(start, end - start + 1);

		if (trimmed.size() >= 2 && trimmed.front() == '"' && trimmed.back() == '"') {
			return trimmed.substr(1, trimmed.size() - 2);
		}

		return trimmed;
	}

std::string Ciri::replace_args(const std::string& input) const {
	std::regex pattern(R"(\$\((\d+)\))");
	std::smatch match;
	std::string result;
	auto it = input.cbegin();
	while (std::regex_search(it, input.cend(), match, pattern)) {
		result.append(it, match[0].first);
		int idx = std::stoi(match[1].str());
		if (idx >= 0 && idx < static_cast<int>(_args.size()))
			result.append(_args[idx]);
		else
			result.append(match[0].str());
		it = match[0].second;
	}
	result.append(it, input.cend());
	return result;
}

std::string Ciri::replace_refs(const std::string& input) const {
	std::regex pattern(R"(\$\{([^=]+)=([^}]+)\})");
	std::smatch match;
	std::string result;
	std::string invalid = "__ciri_invalid_string123122__INVALID_";
	auto it = input.cbegin();
	while (std::regex_search(it, input.cend(), match, pattern)) {
		result.append(it, match[0].first);
		std::string ref = INIReader::Get(match[1].str(), match[2].str(), invalid);
		result.append(ref == invalid ? match[0].str() : ref);
		it = match[0].second;
	}
	result.append(it, input.cend());
	return result;
}

std::string Ciri::Get(const std::string& section, const std::string& name, const std::string& default_value) const {
	std::string val = INIReader::Get(section, name, default_value);
	val = replace_refs(val);
	val = replace_args(val);
	return trim(val);
}

std::vector<std::string> Ciri::GetList(const std::string& section, const std::string& name, const std::vector<std::string>& default_value) const {
	std::string val = Get(section, name, "");
	if (val.empty()) return default_value;

	if (val.front() == '(' && val.back() == ')') {
		std::string inner = val.substr(1, val.size() - 2);
		size_t comma = inner.find(',');
		if (comma == std::string::npos) return default_value;

		char* endptr = nullptr;
		std::string count_str = inner.substr(0, comma);
		long count = std::strtol(count_str.c_str(), &endptr, 10);
		if (endptr != count_str.c_str() + count_str.size() || count < 0) return default_value;

		std::string item = trim(inner.substr(comma + 1));

		if (count <= 0) return {};
		return std::vector<std::string>(count, item);
	}

	if (val.front() == '{' && val.back() == '}') {
		val = val.substr(1, val.size() - 2);
		if (val.empty()) return {};  // Empty list case

		std::vector<std::string> result;
		size_t start = 0;
		while (true) {
			size_t comma = val.find(',', start);
			if (comma == std::string::npos) {
				std::string token = val.substr(start);
				if (!token.empty()) result.push_back(trim(token));
				break;
			}
			std::string token = val.substr(start, comma - start);
			result.push_back(trim(token));
			start = comma + 1;
		}
		return result;
	}

	return default_value;
}

std::vector<long> Ciri::GetIntegerList(const std::string& section, const std::string& name, const std::vector<long>& default_value) const {
	std::vector<std::string> items = GetList(section, name, {});
	std::vector<long> result;
	for (const auto& s : items) {
		char* end;
		long val = std::strtol(s.c_str(), &end, 0);
		if (*end != '\0') return default_value;
		result.push_back(val);
	}
	return result;
}

std::vector<unsigned long> Ciri::GetUnsignedList(const std::string& section, const std::string& name, const std::vector<unsigned long>& default_value) const {
	std::vector<std::string> items = GetList(section, name, {});
	std::vector<unsigned long> result;
	for (const auto& s : items) {
		char* end;
		unsigned long val = std::strtoul(s.c_str(), &end, 0);
		if (*end != '\0') return default_value;
		result.push_back(val);
	}
	return result;
}

std::vector<int64_t> Ciri::GetInteger64List(const std::string& section, const std::string& name, const std::vector<int64_t>& default_value) const {
	std::vector<std::string> items = GetList(section, name, {});
	std::vector<int64_t> result;
	for (const auto& s : items) {
		char* end;
		int64_t val = std::strtoll(s.c_str(), &end, 0);
		if (*end != '\0') return default_value;
		result.push_back(val);
	}
	return result;
}

std::vector<uint64_t> Ciri::GetUnsigned64List(const std::string& section, const std::string& name, const std::vector<uint64_t>& default_value) const {
	std::vector<std::string> items = GetList(section, name, {});
	std::vector<uint64_t> result;
	for (const auto& s : items) {
		char* end;
		uint64_t val = std::strtoull(s.c_str(), &end, 0);
		if (*end != '\0') return default_value;
		result.push_back(val);
	}
	return result;
}

std::vector<double> Ciri::GetRealList(const std::string& section, const std::string& name, const std::vector<double>& default_value) const {
	std::vector<std::string> items = GetList(section, name, {});
	std::vector<double> result;
	for (const auto& s : items) {
		char* end;
		double val = std::strtod(s.c_str(), &end);
		if (*end != '\0') return default_value;
		result.push_back(val);
	}
	return result;
}

std::vector<std::string> Ciri::GetStringList(const std::string& section, const std::string& name, const std::vector<std::string>& default_value) const {
	return GetList(section, name, default_value);
}

std::vector<bool> Ciri::GetBooleanList(const std::string& section, const std::string& name, const std::vector<bool>& default_value) const {
	std::vector<std::string> items = GetList(section, name, {});
	std::vector<bool> result;
	for (auto valstr : items) {
		std::transform(valstr.begin(), valstr.end(), valstr.begin(),
				[](const unsigned char& ch) { return static_cast<unsigned char>(::tolower(ch)); });

		if (valstr == "true" || valstr == "yes" || valstr == "on" || valstr == "1")
			result.push_back(true);
		else if (valstr == "false" || valstr == "no" || valstr == "off" || valstr == "0")
			result.push_back(false);
		else
			return default_value;
	}
	return result;
}

int Ciri::Add(const std::string& section, const std::string& name, const std::string& value) {
	std::string key = MakeKey(section, name);
	if (_values[key].size() > 0)
		_values[key] += "\n";
	_values[key] += value;
	return 0;
}

int Ciri::Remove(const std::string& section, const std::string& name) {
	std::string key = MakeKey(section, name);
	return !_values.erase(key);
}

int Ciri::Save(const std::string& output) const{
	std::ofstream out(output);
	if (!out.is_open()) return 1;

	std::string current = "";

	// Print default section
	for (const auto& [key, value] : _values) {
		auto pos = key.find('=');
		if (pos == std::string::npos) return 1; // Something bad
		std::string section = key.substr(0,pos);
		if (!section.empty()) continue; //Skip non default
		std::string name = key.substr(pos+1);
		out << name << " = " << value << "\n";
		if (out.fail()) return 1;
	}

	// Print remainding sections
	for (const auto& [key, value] : _values) {
		auto pos = key.find('=');
		if (pos == std::string::npos) return 1; // Something bad
		std::string section = key.substr(0,pos);
		std::string name = key.substr(pos+1);
		if (section.empty()) continue; //Skip default
		if (section != current) {
			out << "[" << section << "]\n";
			current = section;
		}
		out << name << " = " << value << "\n";
		if (out.fail()) return 1;
	}
	return 0;
}

std::string Ciri::File() const {
	return _file;
}
