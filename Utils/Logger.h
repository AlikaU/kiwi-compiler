#pragma once
#include <string>
#include <iostream>
#include <fstream>
//#include "..\Project1\Token.h"

#define LOG_TOKEN_PATH "../Output/logTokens.txt"
#define LOG_DERIV_PATH "../Output/logDerivation.txt"
#define LOG_DEBUG_PATH "../Output/logDebug.txt"
#define LOG_ERROR_PATH "../Output/logError.txt"
#define LOG_TABLE_PATH "../Output/logTable.txt"

// very simple singleton class for logging

class Logger {

	static std::ofstream logTokens;
	static std::ofstream logDebug;
	static std::ofstream logDerivation;
	static std::ofstream logError;
	static std::ofstream logTable;
	static Logger* instance;
	Logger();
	~Logger();
	

public: 
	static Logger* getLogger();
	
	enum LOG_TYPE {
		TOKEN,
		DEBUG,
		ERROR,
		DERIV, 
		TABLE
	};

	void log(LOG_TYPE t, std::string message);	
};