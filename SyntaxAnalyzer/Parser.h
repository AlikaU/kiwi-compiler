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
	std::list<GSymbol*> derivationParsed;
	std::list<GSymbol*> derivationToBeParsed;
	void printDerivation();
	Token* currentScannedToken;

public: 
	Parser(Scanner* s, ParseTable* t);
	~Parser();
	bool parse();
	void skipErrors(int errorCode);
};