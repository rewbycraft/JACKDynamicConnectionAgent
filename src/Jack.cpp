//
// Created by rewbycraft on 4/28/15.
//

#include "Jack.h"
#include <easylogging++.h>
#include <util.hpp>
#include <exceptions.hpp>

#define JACKNAME "DynamicConnectionAgent"

void portRegisterCallback(jack_port_id_t portId, int reg, void *arg) {
	Jack *these = (Jack *) arg;
	LOG(DEBUG) << "Port register callback called with port " << portId << " and register " << reg;
	if (reg != 0) {
		//Port is being registered
		these->getEventQueue()->push(new JackPortConnectedEvent(jack_port_by_id((jack_client_t *) these->getClient(), portId)));
	}
}

extern void shutdown();
void jackShutdownCallback(jack_status_t code, const char *reason, void *arg) {
	Jack *these = (Jack *) arg;
	if (!these->isInShutdown()) {
		LOG(ERROR) << "JACK has signalled a shutdown.";
		shutdown();
	}
}

void jackEventThread(Jack *these) {
	std::queue<JackEvent *> *queue = these->getEventQueue();
	while (!these->isInShutdown()) {
		if (queue->empty())
			sleep(1);
		else {
			//Handle event
			auto event = queue->front();
			queue->pop();

			//The reason for this if is that if we're disconnected, we'll just throw away events.
			if (these->getClient() != nullptr)
				switch (event->getEventType()) {
					case PORT_CONNECTED: {
						JackPortConnectedEvent *portEvent = (JackPortConnectedEvent *) event;
						jack_port_t *port = portEvent->getPort();
						std::string clientName = util::split(jack_port_name(port), ":")[0];
						std::string portName = util::split(jack_port_name(port), ":")[1];
						LOG(INFO) << "Got notified of a new port. Client name: " <<
						clientName << " Port name: " << portName << " Scanning for matches...";
						for (auto clientRule: these->getPatterns()) {
							if (std::regex_match(clientName, clientRule->getRegex())) {
								//The client name matched
								LOG(INFO) << "Matched against client-rule " << clientRule << ". Applying port rules...";
								for (auto portRule: clientRule->getPortRules()) {
									if (std::regex_match(portName, portRule->getRegex())) {
										LOG(INFO) <<
										"Matched against port-rule " << portRule << ". Applying rule...";
										if (portRule->getShouldDisconnect())
											jack_port_disconnect((jack_client_t *) these->getClient(), port);
										jack_connect((jack_client_t *) these->getClient(), jack_port_name(port), portRule->getTarget().c_str());
									}
								}
							}
						}
						break;
					};

					default:
						break;
				}

			delete event;
		}
	}
}

void Jack::signalAllConnected() {
	if (jackClient == nullptr)
		return;
	const char **ports = jack_get_ports(jackClient, ".*", ".*", 0);
	int i = 0;
	char *current = (char *) ports[i];
	while (current != NULL) {
		LOG(INFO) << "Forcing connect event for port: " << current;
		eventQueue.push(new JackPortConnectedEvent(jack_port_by_name(jackClient, current)));
		i++;
		current = (char *) ports[i];
	}
}


void Jack::connect() {
	jack_status_t status;

	LOG(DEBUG) << "Connecting to jack...";
	jackClient = jack_client_open(JACKNAME, JackNoStartServer , &status);

	if ((jackClient == nullptr) || (jackClient == NULL) || (jackClient == 0)) {
		LOG(ERROR) << "Could not connect to JACK. Failed with status " << status << ". JACK server failure: " << ((status & JackServerFailed) ? "true" : "false");
		throw JackException("Server Failure.");
	}
	LOG(DEBUG) << "Connected!";

	LOG(DEBUG) << "Setting handlers...";
	if (jack_set_port_registration_callback(jackClient, portRegisterCallback, this) != 0) {
		LOG(ERROR) << "Failed to register port callback.";
		jack_deactivate(jackClient);
		throw JackException("Callback Failure");
	}
	jack_on_info_shutdown(jackClient, jackShutdownCallback, this);

	LOG(DEBUG) << "Activating jack client...";
	jack_activate(jackClient);
}

void Jack::disconnect() {
	if (jackClient == nullptr)
		return;
	LOG(DEBUG) << "Deactivating jack client...";
	jack_deactivate(jackClient);
	LOG(DEBUG) << "Closing jack client...";
	jack_client_close(jackClient);
	jackClient = nullptr;
}

Jack::Jack() {
	eventThread = new std::thread(jackEventThread, this);
}

Jack::~Jack() {
	if (eventThread == nullptr)
		return;
	if (eventThread->joinable()) {
		inShutdown = true;
		eventThread->join();
	}
	delete eventThread;
	eventThread = nullptr;
}
