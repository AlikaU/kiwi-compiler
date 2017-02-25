#include "Logger.h"
#include <iostream>
#include <fstream>

Logger* Logger::instance = NULL;
std::ofstream Logger::logTokens;
std::ofstream Logger::logDebug;
std::ofstream Logger::logDerivation;
std::ofstream Logger::logError;

Logger::Logger() {
	// erase log files from previous run
	remove("logTokens.txt");
	remove("logDerivation.txt");
	remove("logDebug.txt");
	remove("logError.txt");
	
}

Logger::~Logger() {

}

void Logger::log(LOG_TYPE t, std::string message) {
	std::ofstream logTokens("logTokens.txt", std::fstream::out | std::fstream::app);
	std::ofstream logDebug("logDebug.txt", std::fstream::out | std::fstream::app);
	std::ofstream logDerivation("logDerivation.txt", std::fstream::out | std::fstream::app);
	std::ofstream logError("logError.txt", std::fstream::out | std::fstream::app);
	
	switch (t) {
	case TOKEN: 
		if (logTokens.is_open()) {
			logTokens << "\nTOKEN: " << message;
		}
		else { std::cout << "Failed to print to logTokens.txt because file is closed"; }
		if (logDebug.is_open()) {
			logDebug << "\nTOKEN: " << message;
		}
		else { std::cout << "Failed to print to logDebug.txt because file is closed"; }
		break;
	case DEBUG:
		if (logDebug.is_open()) {
			logDebug << "\nDEBUG: " << message;
		}
		else { std::cout << "Failed to print to logDebug.txt because file is closed"; }
		break;
	case DERIVATION:
		if (logDerivation.is_open()) {
			logDerivation << message;
		}
	case ERROR:
		if (logError.is_open()) {
			logError << "\nERROR: " << message;
		}
		else { std::cout << "Failed to print to logError.txt because file is closed"; }
		if (logDebug.is_open()) {
			logDebug << "\nERROR: " << message;
		}
		else { std::cout << "Failed to print to logDebug.txt because file is closed"; }
		break;
	}	
	logTokens.flush();
	logTokens.close();
	logDebug.flush();
	logDebug.close();
	logDerivation.flush();
	logDerivation.close();
	logError.flush();
	logError.close();
}

Logger* Logger::getLogger() {
	if (instance == NULL) {
		instance = new Logger();
	}
	return instance;
}