#include "Parser.h"

Parser::Parser(Scanner* s, ParseTable* t) {
	scanner = s;
	table = t;
	error = false;

}

bool Parser::parse() {

	std::cout << "\nParsing begins!";

	GTerminal dollarsign(GTerminal::DOLLAR_SIGN);
	parsingStack.push(&dollarsign);

	GNonTerminal prog(GNonTerminal::prog);
	parsingStack.push(&prog);
	derivationToBeParsed.push_front(&prog);

	Token* scannedToken = scanner->getNextToken();

	while ( !(parsingStack.top()->isDollarSign()) ) {

		printDerivation();

		GSymbol* topSymbol = parsingStack.top();

		// Terminal on top of stack
		if (topSymbol->isTerminal()) {
			GTerminal term(scannedToken);

			// if the scanned token is the same as the one we have on the top of the stack
			if (static_cast<GTerminal*>(topSymbol)->getType() == term.getType()) {
				parsingStack.pop();
				derivationParsed.push_back(new GTerminal(&term));
				derivationToBeParsed.pop_front();
				scannedToken = scanner->getNextToken();
				if (!scannedToken) {
					break;
				}
			}
			else {
				std::cout << "\nParsing error encountered at token " << term.getValue()
					<< " at line " << term.getPosition().first << ", column " << term.getPosition().second;
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
				std::cout << "\nParsing error encountered at token " << term.getValue()
					<< " at line " << term.getPosition().first << ", column " << term.getPosition().second;
				skipErrors();
				error = true;
			}
		}

		
	} // end while

	if (scannedToken || error) {
		return false;
	} 
	return true;
}

void Parser::printDerivation() {
	std::cout << "\n\n";
	for (GSymbol* symbol : derivationParsed) {
		std::cout << static_cast<GTerminal*>(symbol)->getValue() << " ";
	}
	for (GSymbol* symbol : derivationToBeParsed) {
		if (symbol->isTerminal()) {
			std::cout << GTerminal::getTerminalTypeString(static_cast<int>((static_cast<GTerminal*>(symbol)->getType())));
		}
		else {
			std::cout << GNonTerminal::getNonTerminalTypeString(static_cast<int>((static_cast<GNonTerminal*>(symbol)->getType())));
		}
		std::cout << " ";
	}
}

void Parser::inverseRHSMultiplePush(int ruleNo) {

	// make a copy of rule
	std::list<GSymbol*> rule(table->getRule(ruleNo));

	if (static_cast<GNonTerminal*>(rule.front())->getType() == static_cast<GNonTerminal*>(derivationToBeParsed.front())->getType()) {
		derivationToBeParsed.pop_front();
	}
	else {
		std::cerr << "Rule's LHS does not correspond to beginning of derivation list!!Something is very wrong... ";
		std::getchar();
		exit(1);
	}
	rule.pop_front(); // discard first element, because it's LHS

	while (!(rule.empty())) {
		if (! (rule.back()->isEpsilon()) ) {
			parsingStack.push(rule.back());
			derivationToBeParsed.push_front(rule.back());
		}
		rule.pop_back();
	}	
}

void Parser::skipErrors() {
	std::getchar();
	exit(1);
}
