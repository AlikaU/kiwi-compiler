#pragma once
#include "../Project1/Scanner.h"
#include "GSymbol.h"
#include "GTerminal.h"
#include "GNonTerminal.h"
#include "../Project1/Token.h"
#include <stack>

class Parser {

	Scanner* scanner;
	std::stack<GSymbol*> parsingStack;

public: 
	Parser(Scanner* s);

	bool parse();
};