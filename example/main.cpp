#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include "Ciri.h"

int main() {
	std::cout << "Input a number (0 - 255): ";
	int oranges;
	std::cin >> oranges;

	if (oranges > 255 || oranges < 0) {
		std::cout << "Invalid value, now you have 3\n";
		oranges = 3;
	}


	const char* user = std::getenv("USER"); // On Unix-like systems
	if (!user) user = std::getenv("USERNAME"); // On Windows
						   
	std::vector<std::string> args;


	args.emplace_back(user);
	args.emplace_back(std::to_string(oranges));

	Ciri ciri("example/test.ini", args);

	// Test simple Get with substitution
	std::cout << "\n" << ciri.Get("example", "text", "default") << "\n";

	// Test GetList with normal list
	auto list1 = ciri.GetList("lists", "buy", {});
	std::cout << "To buy: ";
	for (const auto& s : list1) std::cout << s << " ";
	std::cout << "\n";

	// Test GetList with repetition list
	auto list2 = ciri.GetList("lists", "owned", {"default"});
	std::cout << "Owned: ";
	for (const auto& s : list2) std::cout << s << " ";
	std::cout << "\n";

	return 0;
}

