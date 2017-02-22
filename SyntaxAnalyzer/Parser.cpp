#include "Parser.h"

Parser::Parser(Scanner* s) {
	scanner = s;
}

bool Parser::parse() {

	GTerminal dollarsign(GTerminal::DOLLAR_SIGN);
	parsingStack.push(&dollarsign);

	GNonTerminal prog(GNonTerminal::prog);
	parsingStack.push(&prog);

	Token* scannedToken = scanner->getNextToken();

	while ( !(parsingStack.top()->isDollarSign()) ) {
		GSymbol* topSymbol = parsingStack.top();

		if (topSymbol->isTerminal()) {

		}

		// NonTerminal
		else {

		}
	}
}