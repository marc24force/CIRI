#include <iostream>
#include <filesystem>
#include <algorithm>
#include <cctype>   // for ::isdigit
#include <vector>

#include "ciri.h"

namespace fs = std::filesystem;

int main() {
	std::vector<std::string> args;
	std::string input;


	std::cout << "Enter redundancy (0-10): ";
	std::getline(std::cin, input);

	// check if input is a number between 0 and 10
	if (!input.empty() && std::all_of(input.begin(), input.end(), ::isdigit)) {
		int value = std::stoi(input);
		if (value >= 0) args.push_back(value < 10 ? input : "10");
	}

	Ciri ciri(fs::path("example/test.ini"), args);

	// User
	std::cout << "User: " << ciri.getString("database", "user") << "\n";
	// Test existing lists
	auto hosts = ciri.getStringList("server", "hosts");
	std::cout << "Server hosts:\n";
	for (auto& h : hosts) std::cout << "- " << h << "\n";

	auto modes = ciri.getStringList("server", "modes");
	std::cout << "Server modes:\n";
	for (auto& m : modes) std::cout << "- " << m << "\n";

	auto replicas = ciri.getStringList("database", "replicas");
	std::cout << "Database replicas: (";
	for (size_t i = 0; i < replicas.size(); ++i) {
		if (i > 0) std::cout << ", ";
		std::cout << replicas[i];
	}
	std::cout << ")\n";

	// Add a new list
	ciri.add("server", "new_list", "alpha");
	ciri.add("server", "new_list", "{beta}");
	auto new_list = ciri.getStringList("server", "new_list");
	std::cout << "New list:\n";
	for (auto& n : new_list) std::cout << "- " << n << "\n";

	return 0;
}

