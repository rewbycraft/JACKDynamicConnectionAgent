//
// Created by rewbycraft on 4/28/15.
//

#ifndef JACKDYNCONAGENT_JACK_H
#define JACKDYNCONAGENT_JACK_H

#include <vector>
#include <queue>
#include <PatternClasses.hpp>
#include <jack/jack.h>
#include <thread>

enum JackEventType { NONE, PORT_CONNECTED };
class JackEvent {
public:
	virtual JackEventType getEventType() {
		return NONE;
	}
};

class JackPortConnectedEvent : public JackEvent {
	jack_port_t *port;

public:
	JackPortConnectedEvent(jack_port_t *port) : port(port) { }

	jack_port_t* getPort() const {
		return port;
	}

	virtual JackEventType getEventType() {
		return PORT_CONNECTED;
	}
};

class Jack {
	std::vector<PatternClientMatch*> clientPatterns;
	jack_client_t *jackClient = nullptr;
	bool inShutdown = false;
	std::queue<JackEvent*> eventQueue;
	std::thread* eventThread = nullptr;
public:
	Jack();
	~Jack();

	bool isInShutdown() const {
		return inShutdown;
	}

	std::queue<JackEvent *>* getEventQueue() {
		return &eventQueue;
	}

	const jack_client_t *getClient() const { return jackClient; }

	void connect();
	void disconnect();
	void signalAllConnected();

	void registerPattern(const PatternClientMatch *pattern) {
		clientPatterns.push_back((PatternClientMatch*)pattern);
	}

	const std::vector<PatternClientMatch*> getPatterns() const { return clientPatterns; }
};


#endif //JACKDYNCONAGENT_JACK_H
