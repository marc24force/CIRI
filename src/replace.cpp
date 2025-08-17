#include <regex>
#include "ciri.h"

std::string Ciri::_replace_args(const std::string& input) const {
    std::regex pattern(R"(\$(\d+)|\$\((\d+)(:([^\)]+))?\))");
    std::smatch match;
    std::string result;
    auto it = input.cbegin();

    while (std::regex_search(it, input.cend(), match, pattern)) {
        result.append(it, match[0].first);

        int idx = -1;
        std::string def;

        if (match[1].matched) { // $N
            idx = std::stoi(match[1].str());
        } else if (match[2].matched) { // $(N(:DEFAULT)?)
            idx = std::stoi(match[2].str());
            def = match[4].matched ? match[4].str() : "";
        }

        if (idx-- == 0) result.append(_dir.string());
	else if (idx >= 0 && idx < (int)(_args.size())) result.append(_args[idx]);
        else if (!def.empty()) result.append(def);
        else result.append(match[0].str());

        it = match[0].second;
    }

    result.append(it, input.cend());
    return result;
}


std::string Ciri::_replace_refs(const std::string& input) const {
	std::regex pattern(R"(\$\{([^}]+)#([^}]+)\})");
	std::smatch match;
	std::string result;
	auto it = input.cbegin();

	while (std::regex_search(it, input.cend(), match, pattern)) {
		result.append(it, match[0].first);

		std::string section = match[1].str();
		std::string key = match[2].str();
		if (exists(section, key))
			result.append(_get(section, key));
		else
			result.append(match[0].str());

		it = match[0].second;
	}

	result.append(it, input.cend());
	return result;
}

std::string Ciri::_replace_envs(const std::string& input) const {
    std::regex pattern(R"(\$\{([^}#:]+)(:([^}]*))?\}|\$([A-Za-z_][A-Za-z0-9_]*))");
    std::smatch match;
    std::string result;
    auto it = input.cbegin();

    while (std::regex_search(it, input.cend(), match, pattern)) {
        result.append(it, match[0].first);

        if (match[1].matched) { // ${VAR(:DEFAULT)?}
            std::string var = match[1].str();
            std::string def = match[3].matched ? match[3].str() : "";

            // Treat default as env if exists, else literal
            if (!def.empty()) {
                const char* def_env = std::getenv(def.c_str());
                def = def_env ? def_env : def;
            }

            const char* env = std::getenv(var.c_str());
            result.append(env ? env : def.empty() ? match[0].str() : def);

        } else if (match[4].matched) { // $VAR
            const char* env = std::getenv(match[4].str().c_str());
            result.append(env ? env : match[0].str());
        }

        it = match[0].second;
    }

    result.append(it, input.cend());
    return result;
}

std::string Ciri::_replace_cmds(const std::string& input) const {
	std::regex pattern(R"(\$\[([^\]]*?)\])");
	std::smatch match;
	std::string result;
	auto it = input.cbegin();

	auto exec = [&](const std::string& cmd) -> std::string {
		std::array<char, 128> buffer;
		std::string output;
		std::string full_cmd = "cd " + _dir.string() + " && " + cmd;
		FILE* pipe = popen(full_cmd.c_str(), "r");
		if (!pipe) return "";
		while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
			output += buffer.data();
		}
		pclose(pipe);
		if (!output.empty() && output.back() == '\n') output.pop_back();
		return output;
	};

	while (std::regex_search(it, input.cend(), match, pattern)) {
		result.append(it, match[0].first);
		std::string cmd_output = exec(match[1].str());
		result.append(cmd_output);
		it = match[0].second;
	}
	result.append(it, input.cend());
	return result;
}

