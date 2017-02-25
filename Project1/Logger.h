#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "Token.h"

// very simple singleton class for logging

class Logger {

	static std::ofstream logTokens;
	static std::ofstream logDebug;
	static std::ofstream logDerivation;
	static std::ofstream logError;
	static Logger* instance;
	Logger();
	~Logger();
	

public: 
	static Logger* getLogger();
	
	enum LOG_TYPE {
		TOKEN,
		DEBUG,
		ERROR,
		DERIVATION
	};

	void log(LOG_TYPE t, std::string message);	
};