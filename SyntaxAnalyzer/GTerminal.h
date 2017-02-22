#pragma once
#include "GSymbol.h"
#define NUM_OF_TERMTYPES 37

class GTerminal : public GSymbol {

public:
	GTerminal(Token* t);
	enum TerminalTypes {

		// numbers
		INTNUM = 0,	  // a number that is an integer
		FLOATNUM = 1, // a number that is a float

		// operators
		EQ = 2, // equals
		NE = 3, // not equals
		LT = 4, // less than
		LE = 5, // less than or equals
		GT = 6, // greater than
		GE = 7, // greater than or equals
		DOT = 8,
		PLUS = 9,
		MINUS = 10,
		MULT = 11,
		DIVIDE = 12,
		ASSIGN = 13,
		AND = 14,
		NOT = 15,
		OR = 16,

		// punctuation
		SEMI = 17,
		COMMA = 18,
		OPENPAR = 19,
		CLOSEPAR = 20,
		OPENCURLY = 21,
		CLOSECURLY = 22,
		OPENSQUARE = 23,
		CLOSESQUARE = 24,

		// identifier
		ID = 25,

		// reserved words
		IF = 26,
		THEN = 27,
		ELSE = 28,
		FOR = 29,
		CLASS = 30,
		INTWORD = 31,	// the reserved word "int"
		FLOATWORD = 32,	// the reserved word "float"
		GET = 33,
		PUT = 34,
		RETURN = 35,
		PROGRAM = 36,
		wrongType = -1
	};
	GTerminal(TerminalTypes t);
	bool isTerminal() const { return true; }
	TerminalTypes getType() const { return type; }
	static TerminalTypes stringToType(std::string s);

private:
	std::string value;
	std::pair<int, int> position;
	TerminalTypes type;
	TerminalTypes OperatorToTerminalType(std::string tempValue);
	TerminalTypes PunctuationToTerminalType(std::string tempValue);
	TerminalTypes ReservedWordToTerminalType(std::string tempValue);
	static std::string TerminalTypeStrings[NUM_OF_TERMTYPES];
};