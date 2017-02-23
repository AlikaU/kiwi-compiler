#pragma once
#include "..\Project1\Scanner.h"
#include "GSymbol.h"
#include "GTerminal.h"
#include "GNonTerminal.h"
#include "..\Project1\Token.h"
#include "ParseTable.h"
#include <stack>

class Parser {

	Scanner* scanner;
	ParseTable* table;
	std::stack<GSymbol*> parsingStack;
	bool error;
	void inverseRHSMultiplePush(int ruleNo);

public: 
	Parser(Scanner* s, ParseTable* t);

	bool parse();
	void skipErrors();
};