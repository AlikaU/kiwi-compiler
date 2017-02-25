#include "Parser.h"
#include "..\Project1\Logger.h"

Parser::Parser(Scanner* s, ParseTable* t) {
	scanner = s;
	table = t;
	error = false;

}

bool Parser::parse() {

	std::cout << "\nParsing begins!";
	Logger::getLogger()->log(Logger::DERIVATION, "\nParsing begins!");

	GTerminal dollarsign(GTerminal::DOLLAR_SIGN);
	parsingStack.push(&dollarsign);

	GNonTerminal prog(GNonTerminal::prog);
	parsingStack.push(&prog);
	derivationToBeParsed.push_front(&prog);

	Token* scannedToken = scanner->getNextToken();

	while ( !(parsingStack.top()->isDollarSign()) ) {

		//printDerivation();

		GSymbol* topSymbol = parsingStack.top();

		// Terminal on top of stack
		if (topSymbol->isTerminal()) {
			if (!scannedToken) {
				error = true;
				break;
			}
			GTerminal term(scannedToken);

			// if the scanned token is the same as the one we have on the top of the stack
			if (static_cast<GTerminal*>(topSymbol)->getType() == term.getType()) {
				parsingStack.pop();
				derivationParsed.push_back(new GTerminal(&term));
				derivationToBeParsed.pop_front();
				scannedToken = scanner->getNextToken();				
			}
			else {
				std::cout << "\nParsing error encountered at token " << term.getValue()
					<< " at line " << term.getPosition().first << ", column " << term.getPosition().second;
				Logger::getLogger()->log(Logger::DERIVATION, "\n error encountered at token " + term.getValue()
					+ " at line " + std::to_string(term.getPosition().first) + ", column " + std::to_string(term.getPosition().second));
				skipErrors();
				error = true;
			}
		}

		// NonTerminal on top of stack
		else {
			GNonTerminal* nonterm = static_cast<GNonTerminal*>(topSymbol);
			
			GTerminal term(GTerminal::DOLLAR_SIGN);
			if (scannedToken) {
				term = new GTerminal(scannedToken);				
			}
			
			// if table entry is not an error code
			int ruleNo = table->getRuleNo(nonterm, &term);
			if (ruleNo <= table->getNumRules()) {
				parsingStack.pop();
				inverseRHSMultiplePush(ruleNo);
			}
			else {
				std::cout << "\nParsing error encountered at token " << term.getValue()
					<< " at line " << term.getPosition().first << ", column " << term.getPosition().second;
				Logger::getLogger()->log(Logger::DERIVATION, "\nParsing error encountered at token " + term.getValue()
					+ " at line " + std::to_string(term.getPosition().first) + ", column " + std::to_string(term.getPosition().second));
				skipErrors();
				error = true;
			}
		}

		
	} // end while

	printDerivation();

	if (scannedToken || error || !(parsingStack.top()->isDollarSign())) {
		return false;
	} 
	
	return true;
}

void Parser::printDerivation() {
	Logger::getLogger()->log(Logger::DERIVATION, "\n\n");
	std::cout << "\n\nParsed derivation: ";
	for (GSymbol* symbol : derivationParsed) {
		Logger::getLogger()->log(Logger::DERIVATION, static_cast<GTerminal*>(symbol)->getValue() + " ");
		std::cout << static_cast<GTerminal*>(symbol)->getValue() << " ";
	}
	std::cout << "\n\nTo be parsed derivation: ";
	for (GSymbol* symbol : derivationToBeParsed) {
		if (symbol->isTerminal()) {
			Logger::getLogger()->log(Logger::DERIVATION, GTerminal::getTerminalTypeString(static_cast<int>((static_cast<GTerminal*>(symbol)->getType()))) + " ");
			std::cout << GTerminal::getTerminalTypeString(static_cast<int>((static_cast<GTerminal*>(symbol)->getType()))) << " ";
		}
		else {
			Logger::getLogger()->log(Logger::DERIVATION, GNonTerminal::getNonTerminalTypeString(static_cast<int>((static_cast<GNonTerminal*>(symbol)->getType()))) + " ");
			std::cout << GNonTerminal::getNonTerminalTypeString(static_cast<int>((static_cast<GNonTerminal*>(symbol)->getType()))) << " ";
		}		
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
