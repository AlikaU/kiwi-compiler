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
	void delete_front(std::list<GSymbol*>* derivation) {
		GSymbol* toDelete = derivation->front();
		derivation->pop_front();
		delete toDelete;
	}
	void delete_back(std::list<GSymbol*>* derivation) {
		GSymbol* toDelete = derivation->back();
		derivation->pop_back();
		delete toDelete;
	}
	bool printDeriv;

public: 
	Parser(Scanner* s, ParseTable* t, bool p);
	~Parser();
	bool parse();
	void skipErrors(int errorCode);
};