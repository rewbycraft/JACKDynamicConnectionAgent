//
// Created by rewbycraft on 4/28/15.
//

#include <util.hpp>
#include <regex>
#include <yaml-cpp/yaml.h>
#include <PatternClasses.hpp>
#include <easylogging++.h>
#include <proc/readproc.h>

namespace util {
	//http://stackoverflow.com/a/9437426
	//Credit where credit is due.
	std::vector<std::string> split(const std::string &input, const std::string &regex) {
		//passing -1 as the submatch index parameter performs splitting
		std::sregex_token_iterator first{input.begin(), input.end(), std::regex(regex), -1}, last;
		return {first, last};
	}

	std::vector<PatternClientMatch *> parsePatternNode(const YAML::Node &node) {
		assert(node.IsSequence());
		std::vector<PatternClientMatch *> result;
		for (auto client: node) {
			std::string clientRegex = client["client"].as<std::string>();
			std::vector<PatternPortMatch *> clientPorts;

			assert(client["portrules"].IsSequence());
			for (auto port: (YAML::Node) client["portrules"])
				clientPorts.push_back(new PatternPortMatch(port["name"].as<std::string>(), port["target"].as<std::string>(), ((YAML::Node) port["disconnect"]).as<bool>(true)));

			result.push_back(new PatternClientMatch(clientRegex, clientPorts));
		}

		return result;
	}

	std::vector<int> findPidByRegex(const std::string &regex_str) {
		std::vector<int> result;
		std::regex regex(regex_str);
		PROCTAB *proc = openproc(PROC_FILLCOM | PROC_FILLSTATUS);

		proc_t *proc_info;
		while ((proc_info = readproc(proc, NULL)) != NULL) {
			if (std::regex_search(proc_info->cmd, regex) ||
			    ((proc_info->cmdline != NULL) ? std::regex_match(proc_info->cmdline[0], regex) : false))
				result.push_back(proc_info->ppid);
			free(proc_info);
		}

		closeproc(proc);
		return result;
	}

	std::vector<std::regex> parseRegex(const std::string &regex_str) {
		std::vector<std::regex> result;
		std::smatch matches;
		std::regex reg("%.*%");
		if (std::regex_search(regex_str, matches, reg) && (matches.size() > 0)) {
			std::ssub_match match = matches[0];
			std::string::size_type n = regex_str.find(match.str());
			for (int pid : findPidByRegex(std::string(match.str()).substr(1, match.str().length() - 2)))
				result.push_back(std::regex(std::string(regex_str).replace(n, match.str().length(), std::to_string(pid))));
		} else
			result.push_back(std::regex(regex_str));
		return result;
	}
}