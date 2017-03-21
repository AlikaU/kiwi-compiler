#include "Parser.h"
#include "..\..\Utils\Logger.h"

Parser::Parser(Scanner* s, ParseTable* t, bool p, bool c) {
	scanner = s;
	table = t;
	error = false;
	printDeriv = p;
	printDerivToConsole = c;

}

Parser::~Parser() {
	for (GSymbol* symbol : derivationParsed) {
		if (symbol) {
			delete symbol;
		}
			
	}
	derivationParsed.clear();
	for (GSymbol* symbol : derivationToBeParsed) {
		if (symbol) {
			delete symbol;
		}
			
	}
	derivationToBeParsed.clear();
}

bool Parser::parse() {

	GTerminal dollarsign(GTerminal::DOLLAR_SIGN);
	parsingStack.push(&dollarsign);

	GNonTerminal prog(GNonTerminal::prog);
	parsingStack.push(&prog);
	derivationToBeParsed.push_front(prog.clone());

	currentScannedToken = scanner->getNextToken();

	while ( !(parsingStack.top()->isDollarSign()) ) {

		if (printDeriv) {
			printDerivation();
		}		
		if (printDerivToConsole) {
			printDerivationToConsole();
		}
		//printSemanticStack();
	
		GSymbol* topSymbol = parsingStack.top();

		// Terminal on top of stack
		if (topSymbol->getSymbolType() == GSymbol::terminal) {
			if (currentScannedToken->getType() == Token::DOLLAR_SIGN) {
				error = true;
				break;
			}
			GTerminal term(currentScannedToken);
			if (term.getPosition().first == 8 && term.getPosition().second == 1) {
				//std::cout << "here";
			}

			// if the scanned token is the same as the one we have on the top of the stack
			if (static_cast<GTerminal*>(topSymbol)->getType() == term.getType()) {
				parsingStack.pop();
				derivationParsed.push_back(new GTerminal(&term));
				delete_front(&derivationToBeParsed);
				if (isCollecting) {
					semanticStack.push_back(new GTerminal(currentScannedToken));
				}
				currentScannedToken = scanner->getNextToken();				
			}
			else {
				//std::cout << "\nParsing error encountered at token '" << term.getValue()
				//	<< "' at line " << term.getPosition().first << ", column " << term.getPosition().second;
				Logger::getLogger()->log(Logger::ERROR, "Parser.cpp: error encountered at token '" + term.getValue()
					+ "' at line " + std::to_string(term.getPosition().first) + ", column " + std::to_string(term.getPosition().second));
				
				// if there's a terminal on top of stack and it can't be matched, pop it
				// pop error code is numRules + 1
				skipErrors(table->getNumRules() + 2);
				if (currentScannedToken->getType() == Token::DOLLAR_SIGN) {
					std::cout << "\nEof reached!";
					break;
				}
			}
		}

		// NonTerminal on top of stack
		else if(topSymbol->getSymbolType() == GSymbol::nonTerminal){
			GNonTerminal* nonterm = static_cast<GNonTerminal*>(topSymbol);
			
			GTerminal* term = new GTerminal(currentScannedToken);
			
			// if table entry is not an error code
			int ruleNo = table->getRuleNo(nonterm, term);
			if (ruleNo <= table->getNumRules()) {
				parsingStack.pop();
				inverseRHSMultiplePush(ruleNo);
			}
			else {
				//std::cout << "\nParsing error encountered at token '" << term->getValue()
				//	<< "' at line " << term->getPosition().first << ", column " << term->getPosition().second;
				Logger::getLogger()->log(Logger::ERROR, "Parser.cpp: Parsing error encountered at token '" + term->getValue()
					+ "' at line " + std::to_string(term->getPosition().first) + ", column " + std::to_string(term->getPosition().second));
				skipErrors(ruleNo);
				if (currentScannedToken->getType() == Token::DOLLAR_SIGN) {
					std::cout << "\nEof reached!";
					break;
				}
			}
			delete term;
		}

		// SemanticAction on top of parsing stack
		else {
			processSemanticAction(static_cast<SemanticAction*>(topSymbol));			
			parsingStack.pop();
		}
		
	} // end while

	if (printDeriv) {
		printDerivation();
	}
	if (printDerivToConsole) {
		printDerivationToConsole();
	}

	if (currentScannedToken->getType() != Token::DOLLAR_SIGN || error || !(parsingStack.top()->isDollarSign())) {
		return false;
	} 
	if (globalSymbolTable) {
		globalSymbolTable->print();
	}
	return true;
}

void Parser::processSemanticAction(SemanticAction* action) {
	
	switch (action->getType()) {
	case (SemanticAction::createProgTable):
		globalSymbolTable = new SymbolTable(GLOBAL_TABLE_NAME);
		currentScope = globalSymbolTable;
		break;
	case (SemanticAction::startCollecting):
		isCollecting = true;
		break;
	case (SemanticAction::stopCollecting):
		isCollecting = false;
		break;
	case (SemanticAction::createSemanticClassAndTable):	
		createSemanticClassAndTable();
		break;	
	case (SemanticAction::calculateClassSize):
		break;
	case (SemanticAction::scopeIn):
		scopeIn();
		break;
	case (SemanticAction::scopeOut):
		scopeOut();
		break;
	case (SemanticAction::createSemanticVariable):
	{
		SemanticVariable* varRecord = createSemanticVariable(false);
		varRecord->setDeclared();
		currentScope->insert(varRecord->getIdentifier(), varRecord);
	}		
		break;
	case (SemanticAction::createSemanticFunctionAndTable):
		createSemanticFunctionAndTable();
		break;
	case (SemanticAction::processAssignment):
		processAssignment();
		break;
	}
	//semanticStack.push_back(action);
}

void Parser::scopeIn() {
	if (!(semanticStack.empty())) {
		GSymbol* symbol = semanticStack.back();
		if (symbol->getSymbolType() == GSymbol::semanticRecord) {
			SemanticRecordHolder* hold = static_cast<SemanticRecordHolder*>(symbol);
			if (hold->getRecord()->getSemanticType() == SemanticRecord::FUNCTION) {
				currentScope = static_cast<SemanticFunction*>(hold->getRecord())->getLocalSymbolTable();
			}
			else if(hold->getRecord()->getSemanticType() == SemanticRecord::CLASS_T) {
				currentScope = static_cast<SemanticClass*>(hold->getRecord())->getLocalSymbolTable();
			}
			else {
				std::cout << "cannot scope in, top of semantic stack is not a function nor a class";
			}
		}
		else {
			std::cout << "cannot scope in, top of semantic stack is not a record";
		}		
	}
	else {
		std::cout << "cannot scope in, semantic stack empty";
	}
}

void Parser::scopeOut() {
	if (currentScope != globalSymbolTable) {
		currentScope = currentScope->getParent();
	}
	if (!(semanticStack.empty())) {
		semanticStack.pop_back();
	}
}

bool Parser::processArraySizeList() {
	currentArraySizeList.clear();
	
	GTerminal* term = getNextTerminalFromSemanticStack();
	if (term == NULL) {
		return false;
	}	

	// we expect arraySize tokens in following order: "]", "INT", "[" 
	GTerminal::TerminalTypes expectedTypes[3] = { GTerminal::CLOSESQUARE, GTerminal::INTNUM, GTerminal::OPENSQUARE };
	int expectedType = 0;

	// get arraySizeList tokens
	while (term->getType() == GTerminal::OPENSQUARE || term->getType() == GTerminal::CLOSESQUARE || term->getType() == GTerminal::INTNUM) {
		if (term->getType() != expectedTypes[expectedType]) {
			logSymbolErrorAndSetFlag(GTerminal::getTerminalTypeString(expectedTypes[expectedType]));
			return false;
		}
		currentArraySizeList.push_front(new GTerminal(term));
		semanticStack.pop_back();
		term = getNextTerminalFromSemanticStack();
		if (term == NULL) { return false; }
		expectedType = ++expectedType % 3;
	}

	return true;
}

bool Parser::processAssignment() {
	GTerminal* term = getNextTerminalFromSemanticStack();
	if (term == NULL) {
		return false;
	}

}

void Parser::createSemanticFunctionAndTable() {	
	
	// get fParams
	GTerminal* term = getNextTerminalFromSemanticStack();
	if (term == NULL) { return; }
	if (term->getType() != GTerminal::CLOSEPAR) {
		logSymbolErrorAndSetFlag("')'");
		return;
	}
	semanticStack.pop_back();
	std::list<SemanticVariable*>* paramList = new std::list<SemanticVariable*>();
	term = getNextTerminalFromSemanticStack();
	if (term == NULL) { return; }
	while (term->getType() == GTerminal::INTWORD || term->getType() == GTerminal::FLOATWORD || term->getType() == GTerminal::ID ||
		term->getType() == GTerminal::OPENSQUARE || term->getType() == GTerminal::CLOSESQUARE || term->getType() == GTerminal::INTNUM ||
		term->getType() == GTerminal::COMMA) {
		if (term->getType() == GTerminal::COMMA) {
			semanticStack.pop_back();
			term = getNextTerminalFromSemanticStack();
			if (term == NULL) { return; }
			continue;
		}
		SemanticVariable* varRecord = createSemanticVariable(true);
		if (varRecord == NULL) { return; }
		varRecord->setDeclared();
		paramList->push_back(varRecord);

		term = getNextTerminalFromSemanticStack();
		if (term == NULL) { return; }
	}
	if (term->getType() != GTerminal::OPENPAR) {
		logSymbolErrorAndSetFlag("'('");
		return;
	}
	semanticStack.pop_back();

	// get ID token
	term = getNextTerminalFromSemanticStack();
	if (term == NULL) { return; }
	if (term->getType() != GTerminal::ID) {
		logSymbolErrorAndSetFlag("identifier");
		return;
	}
	GTerminal* funcIDtoken = new GTerminal(term);
	semanticStack.pop_back();

	// get returnType token
	term = getNextTerminalFromSemanticStack();
	if (term == NULL) { return; }

	if (term->getType() != GTerminal::INTWORD && term->getType() != GTerminal::FLOATWORD && term->getType() != GTerminal::ID) {
		logSymbolErrorAndSetFlag("type (int, float or id)");
		return;
	}
	GTerminal* funcRetTypeToken = new GTerminal(term);
	semanticStack.pop_back();

	SemanticRecord::SemanticStructure recordStructure;
	bool isInt = false;
	int arrayDimension = 0;
	if (funcRetTypeToken->getType() == GTerminal::INTWORD) { recordStructure = SemanticRecord::SIMPLE; isInt = true; }
	else if (funcRetTypeToken->getType() == GTerminal::FLOATWORD) { recordStructure = SemanticRecord::SIMPLE; }
	else { recordStructure = SemanticRecord::CLASS_S; }
	if (!(currentArraySizeList.empty())) {
		recordStructure = SemanticRecord::ARRAY;
		arrayDimension = currentArraySizeList.size() / 3;
	}
	SymbolTable* functionTable = new SymbolTable(currentScope, funcIDtoken->getValue());
	SemanticFunction* funcRecord = new SemanticFunction(funcIDtoken->getValue(), recordStructure, arrayDimension, 0, paramList, functionTable);
	currentScope->insert(funcRecord->getIdentifier(), funcRecord);
	funcRecord->setDeclared();
	semanticStack.push_back(new SemanticRecordHolder(funcRecord));
}

void Parser::createSemanticClassAndTable() {
	GSymbol* symbol = semanticStack.back();
	if (symbol->getSymbolType() == GSymbol::terminal) {
		GTerminal* term = static_cast<GTerminal*>(symbol);
		std::string className = term->getValue();
		SymbolTable* classTable = new SymbolTable(currentScope, className);
		SemanticClass* classRecord = new SemanticClass(className, 0, 0, classTable);
		currentScope->insert(className, classRecord);
		currentScope = classTable;
		classRecord->setDeclared();
		semanticStack.pop_back();
	}
	else {
		Logger::getLogger()->log(Logger::ERROR, "Expected terminal on top of stack, but there is something else! Something went really wrong.");
		std::cout << "Expected terminal on top of stack, but there is something else! Something went really wrong.";
		error = true;
	}
}

void Parser::logSymbolErrorAndSetFlag(std::string symbol) {
	Logger::getLogger()->log(Logger::ERROR, "Expected " + symbol + " on top of stack, but there is something else! Something went really wrong.");
	std::cout << "Expected " + symbol + " on top of stack, but there is something else! Something went really wrong.";
	error = true;
}

SemanticVariable* Parser::createSemanticVariable(bool fParam) {
	if (!processArraySizeList()) {
		return NULL;
	}
	GTerminal* term = getNextTerminalFromSemanticStack();

	// get ID token
	if (term->getType() != GTerminal::ID) {
		logSymbolErrorAndSetFlag("identifier");
		return NULL;
	}
	GTerminal* varIDToken = new GTerminal(term);
	semanticStack.pop_back();
	term = getNextTerminalFromSemanticStack();
	if (term == NULL) { return NULL; }

	// get type token
	if (term->getType() != GTerminal::INTWORD && term->getType() != GTerminal::FLOATWORD && term->getType() != GTerminal::ID) {
		logSymbolErrorAndSetFlag("type (int, float or id)");
		return NULL;
	}
	GTerminal* varTypeToken = new GTerminal(term);
	semanticStack.pop_back();

	SemanticRecord::SemanticRecordType recordType;
	SemanticRecord::SemanticStructure recordStructure;
	bool isInt = false;
	int arrayDimension = 0;
	if (varTypeToken->getType() == GTerminal::INTWORD) { recordType = SemanticRecord::INT; recordStructure = SemanticRecord::SIMPLE; isInt = true; }
	else if (varTypeToken->getType() == GTerminal::FLOATWORD) { recordType = SemanticRecord::FLOAT; recordStructure = SemanticRecord::SIMPLE;}
	else { recordType = SemanticRecord::CLASS_T; recordStructure = SemanticRecord::CLASS_S;	}
	if (!(currentArraySizeList.empty())) {
		recordStructure = SemanticRecord::ARRAY;
		arrayDimension = currentArraySizeList.size() / 3;
	}
	SemanticVariable::VariableKind varKind;
	if (fParam) {
		varKind = SemanticVariable::PARAM;
	}
	else {
		varKind = SemanticVariable::NORMAL;
	}

	SemanticVariable* varRecord = new SemanticVariable(varIDToken->getValue(), recordType, recordStructure, arrayDimension, 0, varKind);
	return varRecord;
}



GTerminal* Parser::getNextTerminalFromSemanticStack() {
	GSymbol* symbol = semanticStack.back();
	if (!(symbol->getSymbolType() == GSymbol::terminal)) {
		logSymbolErrorAndSetFlag("terminal");
		return NULL;
	}
	return static_cast<GTerminal*>(symbol);
}

void Parser::printSemanticStack() {
	Logger::getLogger()->log(Logger::DERIV, "\nSemantic stack:");
	if (semanticStack.empty()) {
		return;
	}
	for (GSymbol* symbol : semanticStack) {
		if (symbol->getSymbolType() == GSymbol::semanticAction) {
			Logger::getLogger()->log(Logger::DERIV, static_cast<SemanticAction*>(symbol)->getSemanticTypeString() + " ");
		}
		else if (symbol->getSymbolType() == GSymbol::semanticRecord) {
			std::string str = static_cast<SemanticRecordHolder*>(symbol)->getRecord()->getIdentifier();
			Logger::getLogger()->log(Logger::DERIV, str + " ");
		}
		else {
			Logger::getLogger()->log(Logger::DERIV, static_cast<GTerminal*>(symbol)->getValue() + " ");
		}
	}
}

void Parser::printDerivation() {
	Logger::getLogger()->log(Logger::DERIV, "\n");
	for (GSymbol* symbol : derivationParsed) {
		Logger::getLogger()->log(Logger::DERIV, static_cast<GTerminal*>(symbol)->getValue() + " ");
	}
	for (GSymbol* symbol : derivationToBeParsed) {
		if (symbol->getSymbolType() == GSymbol::terminal) {
			Logger::getLogger()->log(Logger::DERIV, GTerminal::getTerminalTypeString(static_cast<int>((static_cast<GTerminal*>(symbol)->getType()))) + " ");
			}
		else {
			Logger::getLogger()->log(Logger::DERIV, GNonTerminal::getNonTerminalTypeString(static_cast<int>((static_cast<GNonTerminal*>(symbol)->getType()))) + " ");
			}		
	}
}

void Parser::printDerivationToConsole() {
	std::cout << "\n\nSymbols parsed: ";
	for (GSymbol* symbol : derivationParsed) {
		std::cout << static_cast<GTerminal*>(symbol)->getValue() << " ";
	}
	std::cout << "\nSymbols to be parsed: ";
	for (GSymbol* symbol : derivationToBeParsed) {
		if (symbol->getSymbolType() == GSymbol::terminal) {
			std::cout << GTerminal::getTerminalTypeString(static_cast<int>((static_cast<GTerminal*>(symbol)->getType()))) << " ";
		}
		else {
			std::cout << GNonTerminal::getNonTerminalTypeString(static_cast<int>((static_cast<GNonTerminal*>(symbol)->getType()))) << " ";
		}
	}
}

void Parser::inverseRHSMultiplePush(int ruleNo) {

	// make a copy of rule
	std::list<GSymbol*> rule;
	for (GSymbol* symbol : table->getRule(ruleNo)) {
		rule.push_back(symbol->clone());
	}

	if (static_cast<GNonTerminal*>(rule.front())->getType() == static_cast<GNonTerminal*>(derivationToBeParsed.front())->getType()) {
		delete_front(&derivationToBeParsed);
	}
	else {
		std::cerr << "Rule's LHS does not correspond to beginning of derivation list!!Something is very wrong... ";
		int answer;
		std::cin >> answer;
		exit(1);
	}
	rule.pop_front(); // discard first element, because it's LHS

	// make a copy of rule to put in derivationToBeParsed
	std::list<GSymbol*> derivationToBeParsedRule;
	for (GSymbol* symbol : rule) {
		derivationToBeParsedRule.push_back(symbol->clone());
	}

	while (!(rule.empty())) {
		if (! (rule.back()->isEpsilon())) {

			parsingStack.push(rule.back());

			if (!(rule.back()->getSymbolType() == GSymbol::semanticAction)) {
				derivationToBeParsed.push_front(rule.back());
			}			
		}
		rule.pop_back();
	}	
}


// numRules + 1 -> pop
// numRules + 2 -> scan
void Parser::skipErrors(int errorCode) {
	error = true;

	// pop
	if (errorCode == table->getNumRules() + 1) {

		// error reporting
		GSymbol* symbol = parsingStack.top();
		std::string typeString;
		if (symbol->getSymbolType() == GSymbol::terminal) {
			typeString = GTerminal::getTerminalTypeStringNoQuotes(static_cast<int>(static_cast<GTerminal*>(symbol)->getType()));
		}
		else {
			typeString = GNonTerminal::getNonTerminalTypeString(static_cast<int>(static_cast<GNonTerminal*>(symbol)->getType()));
		}
		Logger::getLogger()->log(Logger::ERROR, "Parser.cpp: Handling error: POP (abandoned trying to parse " + typeString + ")");
		//std::cout << "\nPOP\n";

		// actual pop
		parsingStack.pop();
		delete_front(&derivationToBeParsed);		
	}
	// scan
	else if (errorCode == table->getNumRules() + 2) {
		currentScannedToken = scanner->getNextToken();

		Logger::getLogger()->log(Logger::ERROR, "Parser.cpp: Handling error: SCAN (skipped input token that caused error)");
		//std::cout << "\nSCAN\n";
	}
	else {
		std::cerr << "Wrong error code: " << errorCode << "! Something is very very wrong. Exiting.";
		std::getchar();
	}
}
