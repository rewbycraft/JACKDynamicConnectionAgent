//
// Created by rewbycraft on 4/28/15.
//

#ifndef JACKDYNCONAGENT_PATTERNCLASSES_HPP
#define JACKDYNCONAGENT_PATTERNCLASSES_HPP
#include <string>
#include <regex>

class PatternPortMatch {
	std::string regex;
	std::string target;
	bool shouldDisconnect;

public:
	PatternPortMatch(std::string regex, std::string target, bool shouldDisconnect = true) : regex(regex),
											   shouldDisconnect(shouldDisconnect),
											   target(target) { }

	const std::string &getRegex() const {
		return regex;
	}

	const bool getShouldDisconnect() const {
		return shouldDisconnect;
	}

	const std::string &getTarget() const {
		return target;
	}
};

class PatternClientMatch {
	std::string regex;
	std::vector<PatternPortMatch*> portRules;
public:
	PatternClientMatch(std::string regex, std::vector<PatternPortMatch *> const ports) : regex(regex),
											      portRules(ports) { }

	virtual ~PatternClientMatch() {
		for (auto port: portRules) {
			delete port;
		}
	}


	const std::string &getRegex() const {
		return regex;
	}

	const std::vector<PatternPortMatch *> &getPortRules() const {
		return portRules;
	}
};

#endif //JACKDYNCONAGENT_PATTERNCLASSES_HPP
