//
// Created by rewbycraft on 4/28/15.
//

#ifndef JACKDYNCONAGENT_UTIL_HPP
#define JACKDYNCONAGENT_UTIL_HPP

#include <vector>
#include <string>
#include "PatternClasses.hpp"
#include <yaml-cpp/yaml.h>

namespace util {
	std::vector<std::string> split(const std::string& input, const std::string& regex);

	std::vector<PatternClientMatch*> parsePatternNode(const YAML::Node &node);

	std::vector<int> findPidByRegex(const std::string &regex_str);
}

#endif //JACKDYNCONAGENT_UTIL_HPP
