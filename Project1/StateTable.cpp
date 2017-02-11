#include "StateTable.h"
#include <iostream>
#include <map>
#include "Logger.h"



StateTable::StateTable() {

}

const unsigned int StateTable::TransitionTable[31][21] = {
	{1,2,3,7,8,9,10,11,12,13,14,15,16,17,19,22,24,27,29,30,0 },			// START state
	{1,1,1,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,1,29,29 },		// WORD state
	{29,2,2,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,4,29 },		// INT state
	{29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,4,29 },	// INT_ZERO state
	{29,5,5,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29 },	// SOON_TO_BE_FLOAT state
	{29,5,6,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29 },	// FLOAT state
	{29,5,6,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29 },	// SAW_A_ZERO state
	{29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29 },	// SEMICOLON state
	{29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29 },	// COMMA state
	{29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29 },	// PLUS state
	{ 29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29 ,29 }, // MINUS state
	{ 29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29 ,29 }, // OPENPAR state
	{ 29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29 ,29 }, // CLOSEPAR state
	{ 29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29 ,29 }, // OPENCURLY state
	{ 29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29 ,29 }, // CLOSECURLY state
	{ 29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29 ,29 }, // OPENSQUARE state
	{ 29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29 ,29 }, // CLOSESQUARE state
	{ 29,29,29,29,29,29,29,29,29,29,29,29,29,18,29,29,29,29,29,29 ,29 }, // ASSIGNMENT state
	{ 29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29 ,29 }, // EQUALS state
	{ 29,29,29,29,29,29,29,29,29,29,29,29,29,29,20,21,29,29,29,29 ,29 }, // DIVIDE state
	{ 29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29 ,29 }, // LINE_COMMENT state
	{ 29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29 ,29 }, // OPEN_COMMENT state
	{ 29,29,29,29,29,29,29,29,29,29,29,29,29,29,23,29,29,29,29,29 ,29 }, // MULTIPLY state
	{ 29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29 ,29 }, // CLOSE_COMMENT state
	{ 29,29,29,29,29,29,29,29,29,29,29,29,29,25,29,29,29,26,29,29 ,29 }, // LESS_THAN state
	{ 29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29 ,29 }, // LESS_OR_EQUALS state
	{ 29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29 ,29 }, // NOT_EQUALS state
	{ 29,29,29,29,29,29,29,29,29,29,29,29,29,28,29,29,29,29,29,29 ,29 }, // GREATER_THAN state
	{ 29,29,29,29,29,29,29,29,29,29,29,29,29,28,29,29,29,29,29,29 ,29 }, // GREATER_OR_EQUALS state
	{ 29,29,29,29,29,29,29,29,29,29,29,29,29,28,29,29,29,29,29,29 ,29 }, // OUT_OF_DFA state
	{ 29,29,29,29,29,29,29,29,29,29,29,29,29,28,29,29,29,29,29,29 ,29 }  // DOT state

};
	
const std::map < char, int > StateTable::CharsToIndices = {
	{ 'l',0 },
	{ 'd',1 },
	{ '0',2 },
	{ ';',3 },
	{ ',',4 },
	{ '+',5 },
	{ '-',6 },
	{ '(',7 },
	{ ')',8 },
	{ '{',9 },
	{ '}',10 },
	{ '[',11 },
	{ ']',12 },
	{ '=',13 },
	{ '/',14 },
	{ '*',15 },
	{ '<',16 },
	{ '>',17 },
	{ '_',18 },
	{ '.',19 },
	{ ' ',20 }
};

const bool StateTable::FinalStates[31]{
	false, true, true, true, false,
	true, false, true, true, true,
	true, true, true, true, true,
	true, true, true, true, true,
	false, false, true, false, true,
	true, true, true, true, false, 
	true
};

const std::map < StateTable::States, Token::TokenTypes > StateTable::FinalStateToTokenType = {
	{ StateTable::States::INT, Token::TokenTypes::INT },
	{ StateTable::States::INT_ZERO, Token::TokenTypes::INT },
	{ StateTable::States::FLOAT, Token::TokenTypes::FLOAT },
	{ StateTable::States::SEMICOLON, Token::TokenTypes::PUNCTUATION },
	{ StateTable::States::COMMA, Token::TokenTypes::PUNCTUATION },
	{ StateTable::States::PLUS, Token::TokenTypes::OPERATOR },
	{ StateTable::States::MINUS, Token::TokenTypes::OPERATOR },
	{ StateTable::States::OPENPAR, Token::TokenTypes::PUNCTUATION },
	{ StateTable::States::CLOSEPAR, Token::TokenTypes::PUNCTUATION },
	{ StateTable::States::OPENCURLY, Token::TokenTypes::PUNCTUATION },
	{ StateTable::States::CLOSECURLY, Token::TokenTypes::PUNCTUATION },
	{ StateTable::States::OPENSQUARE, Token::TokenTypes::PUNCTUATION },
	{ StateTable::States::CLOSESQUARE, Token::TokenTypes::PUNCTUATION },
	{ StateTable::States::ASSIGNMENT, Token::TokenTypes::OPERATOR },
	{ StateTable::States::EQUALS, Token::TokenTypes::OPERATOR },
	{ StateTable::States::DIVIDE, Token::TokenTypes::OPERATOR },
	{ StateTable::States::MULTIPLY, Token::TokenTypes::OPERATOR },
	{ StateTable::States::LESS_THAN, Token::TokenTypes::OPERATOR },
	{ StateTable::States::LESS_OR_EQUALS, Token::TokenTypes::OPERATOR },
	{ StateTable::States::NOT_EQUALS, Token::TokenTypes::OPERATOR },
	{ StateTable::States::GREATER_THAN, Token::TokenTypes::OPERATOR },
	{ StateTable::States::GREATER_OR_EQUALS, Token::TokenTypes::OPERATOR },
	{ StateTable::States::DOT, Token::TokenTypes::OPERATOR }
};

const std::string StateTable::StateStrings[31] = { "START", "WORD", "INT", "INT_ZERO", "SOON_TO_BE_FLOAT", "FLOAT", "SAW_A_ZERO",
"SEMICOLON", "COMMA", "PLUS", "MINUS", "OPENPAR", "CLOSEPAR", "OPENCURLY", "CLOSECURLY", "OPENSQUARE",
"CLOSESQUARE", "ASSIGNMENT", "EQUALS", "DIVIDE", "LINE_COMMENT", "OPEN_COMMENT", "MULTIPLY", "CLOSE_COMMENT",
"LESS_THAN", "LESS_OR_EQUALS", "NOT_EQUALS", "GREATER_THAN", "GREATER_OR_EQUALS", "OUT_OF_DFA", "DOT"
};

enum StateTable::States StateTable::getState(States state, char c) {
	int ascii = (int)c;
	char lookup = c;

	// tab character and new line are equivalent to space for me
	if (ascii == 9 || ascii == 10) { 
		lookup = ' ';
	}

	// all invalid characters will give error
	else if (!isCharLegal(c)) {
		std::string errorString = "char ";
		errorString += c;
		errorString += "(";
		errorString += "ascii value " + std::to_string((int)c) + ") is not legal";
		//Logger::getLogger()->log(Logger::LOG_TYPE::ERROR, errorString);
		return StateTable::States::OUT_OF_DFA;
	}

	// A-Z || a-z
	if (ascii >= 65 && ascii <= 90 || ascii >= 97 && ascii <= 122) { 
		lookup = 'l';
	}

	// 1-9
	if (ascii >= 49 && ascii <= 57) { 
		lookup = 'd';
	}

	int fromStateIndex = static_cast<int>(state);
	try {
		int charIndex = StateTable::CharsToIndices.at(lookup);
		int toStateIndex = TransitionTable[fromStateIndex][charIndex];
		return static_cast<StateTable::States>(toStateIndex);
	}
	catch (std::out_of_range) {
		Logger::getLogger()->log(Logger::LOG_TYPE::ERROR, 
			"Could not convert input character to index! This should not have happened.");
		return StateTable::States::OUT_OF_DFA;
	}
	
}

bool StateTable::isCharLegal(char c) {
	int ascii = (int)c;
	if (ascii <= 31 || ascii >= 33 && ascii <= 39 || ascii == 58 || ascii == 63 || ascii == 64 || ascii == 92 ||
		ascii == 94 || ascii == 96 || ascii == 124 || ascii == 126 || ascii == 127) {
		return false;
	} return true;
}

bool StateTable::isFinalState(StateTable::States s) {
	return FinalStates[static_cast<int>(s)];
}
