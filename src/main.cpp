#include <iostream>
#include <easylogging++.h>
#include <unistd.h>
#include <PatternClasses.hpp>
#include <optionparser.h>
#include <Jack.h>
#include <yaml-cpp/node/parse.h>
#include <util.hpp>
#include <exceptions.hpp>
#include <execinfo.h>

INITIALIZE_EASYLOGGINGPP

#define BACKTRACE 32
#define BTOFFSET 0

using namespace std;

bool keepRunning = false;

void shutdown() {
	LOG(INFO) << "Performing graceful shutdown...";
	if (keepRunning)
		keepRunning = false;
	else {
		LOG(INFO) << "Not inside main loop. Exiting...";
		exit(0);
	}
}

void printStacktrace() {
	void *array[BACKTRACE];
	int size = backtrace(array, BACKTRACE);
	char **symbols = backtrace_symbols(&(array[BTOFFSET - 1]), size - BTOFFSET);
	if (symbols == NULL) {
		LOG(ERROR) << "Failed to get backtrace.";
		return;
	}
	LOG(ERROR) << "Backtrace:";
	for (int i = 0; i < size - BTOFFSET; i++) {
		char *cur = symbols[i];
		LOG(ERROR) << " " << i << ".\t" << cur;
	}
	free(symbols);
}

void signalHandler(int signal) {
	switch (signal) {
		case SIGINT:
		case SIGTERM:
			shutdown();
			break;
		case SIGSEGV:
			LOG(ERROR) << "Caught segmentation fault.";
			printStacktrace();
			exit(1);
		default:
			break;
	}
}

void terminator() {
	exception_ptr ex = current_exception();
	try {
		if (ex) {
			std::rethrow_exception(ex);
		}
	} catch (const std::exception &e) {
		LOG(ERROR) << "Global exception handler caught exception. Message: " << e.what();
		printStacktrace();
		LOG(ERROR) << "Calling exit...";
	}
}

static option::ArgStatus ArgRequired(const option::Option &option, bool) {
	if (option.arg/* && option.name[option.namelen] != 0*/)
		return option::ArgStatus::ARG_OK;
	else
		return option::ArgStatus::ARG_ILLEGAL;
}

int main(int argc, char *argv[]) {
	START_EASYLOGGINGPP(argc, argv);

	std::string configFile = "./config.yml";

	enum optionIndex {
		UNKNOWN, HELP, CONFIG, VERBOSE
	};
	const option::Descriptor usage[] = {
	{UNKNOWN, 0, "",  "",        option::Arg::None, std::string(
	"USAGE: " + std::string(argv[0]) + " [options]\n\nOptions:").c_str()},
	{HELP,    0, "h", "help",    option::Arg::None, "  --help     \tPrint usage and exit."},
	{VERBOSE, 0, "v", "verbose", option::Arg::None, "  --verbose  \tEnable verbose logging."},
	{CONFIG,  0, "c", "config",  ArgRequired,       "  --config   \tOverride default config file."},
	{0,       0, 0,   0,         0,                 0}
	};

	argc -= (argc > 0);
	argv += (argc > 0); // skip program name argv[0] if present
	option::Stats stats(usage, argc, argv);
	option::Option options[stats.options_max], buffer[stats.buffer_max];
	option::Parser parse(usage, argc, argv, options, buffer);

	if (parse.error()) {
		LOG(ERROR) << "Incorrect usage.";
		option::printUsage(std::cout, usage);
		return 1;
	}

	if (options[HELP] || argc == 0) {
		option::printUsage(std::cout, usage);
		return 0;
	}

	if (options[CONFIG])
		configFile = options[CONFIG].arg;

	LOG(INFO) << "Starting JACK Dynamic Connection Agent...";
	Jack jack = Jack();

	LOG(INFO) << "Using config file: " << configFile;

	LOG(DEBUG) << "Registering signal handler...";
	set_terminate(terminator);
	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);
	signal(SIGSEGV, signalHandler); //Not unregistered on purpose.
	signal(SIGUSR1, signalHandler);
	signal(SIGUSR2, signalHandler);

	LOG(INFO) << "Parsing config...";

	auto configNode = YAML::LoadFile(configFile);

	auto patterns = util::parsePatternNode(configNode["patterns"]);

	LOG(INFO) << "Found " << patterns.size() << " client-rules.";

	for (auto pattern : patterns)
		jack.registerPattern(pattern);

	LOG(INFO) << "Setting up JACK...";
	jack.connect();

	if (configNode["processcurrent"].as<bool>(true)) {
		LOG(INFO) << "Forcing all current clients to be processed...";
		jack.signalAllConnected();
	}

	LOG(INFO) << "Startup complete. The JACK Dynamic Connection Agent is now running.";
	keepRunning = true;
	while (keepRunning)
		sleep(1);

	LOG(INFO) << "Shutting down...";
	LOG(DEBUG) << "Unregistering signal handler...";
	signal(SIGINT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	signal(SIGUSR1, SIG_IGN);
	signal(SIGUSR2, SIG_IGN);

	LOG(DEBUG) << "Shutting down JACK...";
	jack.disconnect();

	LOG(INFO) << "Good bye!";
	return 0;
}