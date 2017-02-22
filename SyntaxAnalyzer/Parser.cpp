#include "Parser.h"

Parser::Parser(Scanner* s, ParseTable* t) {
	scanner = s;
	table = t;
	error = false;
}

bool Parser::parse() {

	GTerminal dollarsign(GTerminal::DOLLAR_SIGN);
	parsingStack.push(&dollarsign);

	GNonTerminal prog(GNonTerminal::prog);
	parsingStack.push(&prog);

	Token* scannedToken = scanner->getNextToken();

	while ( !(parsingStack.top()->isDollarSign()) ) {
		GSymbol* topSymbol = parsingStack.top();

		// Terminal on top of stack
		if (topSymbol->isTerminal()) {
			GTerminal term(scannedToken);

			// if the scanned token is the same as the one we have on the top of the stack
			if (static_cast<GTerminal*>(topSymbol)->getType() == term.getType()) {
				parsingStack.pop();
				scannedToken = scanner->getNextToken();
			}
			else {
				skipErrors();
				error = true;
			}
		}

		// NonTerminal on top of stack
		else {
			GNonTerminal* nonterm = static_cast<GNonTerminal*>(topSymbol);
			GTerminal term(scannedToken);

			// if table entry is not an error code
			int ruleNo = table->getRuleNo(nonterm, &term);
			if (ruleNo <= table->getNumRules()) {
				parsingStack.pop();
				inverseRHSMultiplePush(ruleNo);
			}
			else {
				skipErrors();
				error = true;
			}
		}
	} // end while

	if (scannedToken->getType() != GTerminal::DOLLAR_SIGN || error) {
		return false;
	} 
	return true;
}

void Parser::inverseRHSMultiplePush(int ruleNo) {
	std::list<GSymbol*> rule = table->getRule(ruleNo);
	rule.pop_front(); // discard first element, because it's LHS
	while (!rule.empty()) {
		parsingStack.push(rule.back());
		rule.pop_back();
	}	
}

void Parser::skipErrors() {
	
}
