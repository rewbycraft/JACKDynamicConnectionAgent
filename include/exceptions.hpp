//
// Created by rewbycraft on 4/28/15.
//

#ifndef JACKDYNCONAGENT_EXCEPTIONS_HPP
#define JACKDYNCONAGENT_EXCEPTIONS_HPP

#include <exception>

class JackException : public std::exception {
	std::string reason;
public:
	JackException(std::string reason) : reason(reason) { }

	JackException(const char* reason) : reason(reason) { }

	virtual const char *what() const throw() override {
		return this->reason.c_str();
	}
};
#endif //JACKDYNCONAGENT_EXCEPTIONS_HPP
