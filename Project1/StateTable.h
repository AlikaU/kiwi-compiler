#pragma once

#include <map>
#include <string>
#include "Token.h"

class StateTable
{
	static const unsigned int TransitionTable[31][21]; // State transition table
	static const std::map<char, int> CharsToIndices; // Mapping of all possible input chars to table indices

public:

	StateTable();

	enum States : int {
		START = 0,
		WORD = 1,
		INT = 2,
		INT_ZERO = 3,
		SOON_TO_BE_FLOAT = 4,
		FLOAT = 5,
		SAW_A_ZERO = 6,
		SEMICOLON = 7,
		COMMA = 8,
		PLUS = 9,
		MINUS = 10,
		OPENPAR = 11,
		CLOSEPAR = 12,
		OPENCURLY = 13,
		CLOSECURLY = 14,
		OPENSQUARE = 15,
		CLOSESQUARE = 16,
		ASSIGNMENT = 17,
		EQUALS = 18,
		DIVIDE = 19,
		LINE_COMMENT = 20,
		OPEN_COMMENT = 21,
		MULTIPLY = 22,
		CLOSE_COMMENT = 23,
		LESS_THAN = 24,
		LESS_OR_EQUALS = 25,
		NOT_EQUALS = 26,
		GREATER_THAN = 27,
		GREATER_OR_EQUALS = 28,
		OUT_OF_DFA = 29, 
		DOT = 30
	};
	static const bool FinalStates[31];
	static const std::string StateStrings[31];
	static const std::map < StateTable::States, Token::TokenTypes > FinalStateToTokenType;
	
	static States getState(enum States, char);
	static bool isCharLegal(char c);
	static bool isFinalState(States s);
};