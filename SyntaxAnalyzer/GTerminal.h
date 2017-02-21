#pragma once
#include "GSymbol.h"

class GTerminal : public GSymbol {

public:
	GTerminal(Token* t);
	enum TerminalTypes {

		// numbers
		INTNUM,	  // a number that is an integer
		FLOATNUM, // a number that is a float

		// operators
		EQ, // equals
		NE, // not equals
		LT, // less than
		LE, // less than or equals
		GT, // greater than
		GE, // greater than or equals
		DOT,
		PLUS,
		MINUS,
		MULT,
		DIVIDE,
		ASSIGN,
		AND,
		NOT,
		OR,

		// punctuation
		SEMI,
		COMMA,
		OPENPAR,
		CLOSEPAR,
		OPENCURLY,
		CLOSECURLY,
		OPENSQUARE,
		CLOSESQUARE,

		// identifier
		ID,

		// reserved words
		IF,
		THEN,
		ELSE,
		FOR,
		CLASS,
		INTWORD,	// the reserved word "int"
		FLOATWORD,	// the reserved word "float"
		GET,
		PUT,
		RETURN,
		PROGRAM
	};
	bool isTerminal() const { return true; }
	TerminalTypes getType() const { return type; }

private:
	std::string value;
	std::pair<int, int> position;
	TerminalTypes type;
	TerminalTypes OperatorToTerminalType(std::string tempValue);
	TerminalTypes PunctuationToTerminalType(std::string tempValue);
	TerminalTypes ReservedWordToTerminalType(std::string tempValue);
};