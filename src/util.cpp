//
// Created by rewbycraft on 4/28/15.
//

#include <util.hpp>
#include <regex>
#include <yaml-cpp/yaml.h>
#include <PatternClasses.hpp>
#include <easylogging++.h>

namespace util {
	//http://stackoverflow.com/a/9437426
	//Credit where credit is due.
	std::vector <std::string> split(const std::string &input, const std::string &regex) {
		//passing -1 as the submatch index parameter performs splitting
		std::sregex_token_iterator first{input.begin(), input.end(), std::regex(regex), -1}, last;
		return {first, last};
	}

	std::vector<PatternClientMatch*> parsePatternNode(const YAML::Node &node) {
		assert(node.IsSequence());
		std::vector<PatternClientMatch*> result;
		for (auto client: node) {
			std::string clientRegex = client["client"].as<std::string>();
			std::vector<PatternPortMatch*> clientPorts;

			assert(client["portRules"].IsSequence());
			for (auto port: (YAML::Node)client["portRules"])
				clientPorts.push_back(new PatternPortMatch(port["name"].as<std::string>(), port["target"].as<std::string>(), ((YAML::Node)port["disconnect"]).as<bool>(true)));

			result.push_back(new PatternClientMatch(clientRegex, clientPorts));
		}

		return result;
	}

	std::vector<int> findPidByRegex(const std::string &regex_str) {
		std::vector<int> result;
		std::regex regex(regex_str);

		return result;
	}
}