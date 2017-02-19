#pragma once
#include "GSymbol.h"

class GTerminal : public GSymbol {


public:

	enum TerminalTypes {

		// numbers
		INT,
		FLOAT,

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
		INT,
		FLOAT,
		GET,
		PUT,
		RETURN,
		PROGRAM
	};

	bool isTerminal() { return true; }
};