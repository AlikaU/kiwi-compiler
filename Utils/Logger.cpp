//#include "stdafx.h"
#include "Logger.h"
#include <iostream>
#include <fstream>

Logger* Logger::instance = NULL;
std::ofstream Logger::logTokens;
std::ofstream Logger::logDebug;
std::ofstream Logger::logDerivation;
std::ofstream Logger::logError;
std::ofstream Logger::logTable;
std::ofstream Logger::logSemanticError;

Logger::Logger() {
	// erase log files from previous run
	remove(LOG_TOKEN_PATH);
	remove(LOG_DERIV_PATH);
	remove(LOG_DEBUG_PATH);
	remove(LOG_ERROR_PATH);
	remove(LOG_TABLE_PATH);
	remove(LOG_SEMANTIC_ERROR_PATH);
}

Logger::~Logger() {

}

void Logger::log(LOG_TYPE t, std::string message) {
	std::ofstream logToken(LOG_TOKEN_PATH, std::fstream::out | std::fstream::app);
	std::ofstream logDebug(LOG_DEBUG_PATH, std::fstream::out | std::fstream::app);
	std::ofstream logDeriv(LOG_DERIV_PATH, std::fstream::out | std::fstream::app);
	std::ofstream logError(LOG_ERROR_PATH, std::fstream::out | std::fstream::app);
	std::ofstream logTable(LOG_TABLE_PATH, std::fstream::out | std::fstream::app);
	std::ofstream logSemanticError(LOG_SEMANTIC_ERROR_PATH, std::fstream::out | std::fstream::app);
	
	switch (t) {
	case TOKEN: 
		if (logToken.is_open()) {
			logToken << "\nTOKEN: " << message;
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
		else { 
			std::cout << "Failed to print to logDebug.txt because file is closed"; }
		break;
	case DERIV:
		if (logDeriv.is_open()) {
			logDeriv << message;
		}
		else { std::cout << "Failed to print to logDerivation.txt because file is closed"; }
		break;
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
		
	case TABLE:
		if (logTable.is_open()) {
			logTable << message;
		}
		else {
			std::cout << "Failed to print to logTable.txt because file is closed";
		}
		break;

	case SEMANTIC_ERROR:
		if (logSemanticError.is_open()) {
			logSemanticError << message;
		}
		else {
			std::cout << "Failed to print to logSemanticError.txt because file is closed";
		}
		break;
	}
	
}

Logger* Logger::getLogger() {
	if (instance == NULL) {
		instance = new Logger();
	}

	return instance;
}