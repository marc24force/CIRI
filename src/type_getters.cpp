#include "ciri.h"

std::string to_string(const std::string& s) {return s;}
int to_int(const std::string& s) { return std::stoi(s, nullptr, 0); }
unsigned long to_unsigned(const std::string& s) { return std::stoul(s, nullptr, 0); }
float to_float(const std::string& s) { return std::stof(s); }
bool to_bool(const std::string& s) {
	std::string lower;
	for (char c : s) lower += std::tolower(c);
	return (lower == "1" || lower == "true" || lower == "yes" || lower == "on");
}


std::string Ciri::getString(const std::string& section, const std::string& key, const std::string& default_value) const {
	return get<std::string>(section, key, default_value, to_string);
}

int Ciri::getInteger(const std::string& section, const std::string& key, int default_value) const {
	return get<int>(section, key, default_value, to_int);
}

unsigned Ciri::getUnsigned(const std::string& section, const std::string& key, unsigned default_value) const {
	return get<unsigned>(section, key, default_value, to_unsigned);
}

bool Ciri::getBoolean(const std::string& section, const std::string& key, bool default_value) const {
	return get<bool>(section, key, default_value, to_bool);
}

float Ciri::getFloat(const std::string& section, const std::string& key, float default_value) const {
	return get<float>(section, key, default_value, to_float);
}

std::vector<std::string> Ciri::getStringList(const std::string& section, const std::string& key, const std::vector<std::string>& default_value) const {
	return getList(section, key, default_value, to_string);
}

std::vector<int> Ciri::getIntegerList(const std::string& section, const std::string& key, const std::vector<int>& default_value) const {
	return getList<int>(section, key, default_value, to_int);
}

std::vector<unsigned> Ciri::getUnsignedList(const std::string& section, const std::string& key, const std::vector<unsigned>& default_value) const {
	return getList<unsigned>(section, key, default_value, to_unsigned);
}

std::vector<bool> Ciri::getBooleanList(const std::string& section, const std::string& key, const std::vector<bool>& default_value) const {
	return getList(section, key, default_value, to_bool);
}

std::vector<float> Ciri::getFloatList(const std::string& section, const std::string& key, const std::vector<float>& default_value) const {
	return getList(section, key, default_value, to_float);
}
