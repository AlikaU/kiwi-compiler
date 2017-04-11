#include "Parser.h"
#include "..\..\Utils\Logger.h"

#define UNKNOWN_VALUE "Some unknown value"

Parser::Parser(ParseTable* t, bool p, bool c, std::string folder, std::string filename) {
	filepathStr = folder + filename + ".txt";
	scanner = new Scanner(filepathStr.c_str());
	table = t;
	error = false;
	semanticError = false;
	printDeriv = p;
	printDerivToConsole = c;
	insideFinalPass = false;
	codeGen = new CodeGenerator(filename);
	
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

	// 1st pass
	std::cout << "\nDoing first pass";
	insideFinalPass = false;
	bool success = passCode();

	if (!success) return false;
	
	// 2nd pass
	std::cout << "\nDoing second pass";
	insideFinalPass = true;
	delete scanner;
	scanner = new Scanner(filepathStr.c_str());
	semanticStack.clear();
	parsingStack = std::stack<GSymbol*>();
	codeGen->genEntryPoint();
	success = passCode();
	codeGen->genHaltPoint();
	return success;
}

bool Parser::passCode() {

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
			if (term.getPosition().first == 41 && term.getPosition().second == 26) {
				std::cout << "";
			}
			delete lastTerminal;
			lastTerminal = new GTerminal(&term);

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
				if (!insideFinalPass) {
					Logger::getLogger()->log(Logger::ERROR, "Parser.cpp: Syntax error encountered at token '" + term.getValue()
						+ "' at line " + std::to_string(term.getPosition().first) + ", column " + std::to_string(term.getPosition().second));
				}			
				
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
				if (insideFinalPass) {
					Logger::getLogger()->log(Logger::ERROR, "Parser.cpp: Syntax error encountered at token '" + term->getValue()
						+ "' at line " + std::to_string(term->getPosition().first) + ", column " + std::to_string(term->getPosition().second));
				}
				
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
			if (static_cast<SemanticAction*>(topSymbol)->getType() == SemanticAction::processIdNestListIdThenIndiceListOrAParams) {
				std::cout << "";
			}
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

	
	if (globalSymbolTable) {
		globalSymbolTable->print();
	}
	if (currentScannedToken->getType() != Token::DOLLAR_SIGN || error || !(parsingStack.top()->isDollarSign())) {
		return false;
	}

	return true;
}

void Parser::processSemanticAction(SemanticAction* action) {
	
	switch (action->getType()) {
	case (SemanticAction::createProgTable):
		if (!insideFinalPass) {
			globalSymbolTable = new SymbolTable(GLOBAL_TABLE_NAME);
		}
		
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
		if (varRecord != NULL) {
			varRecord->setDeclared();
		}		
		if (!insideFinalPass) {
			currentScope->insert(varRecord->getIdentifier(), varRecord);
		}		
		if (insideFinalPass && varRecord != NULL) {
			codeGen->genVariableDecl(varRecord);
			delete varRecord;
		}		
	}		
		break;
	case (SemanticAction::createSemanticFunctionAndTable):
		createSemanticFunctionAndTable();
		break;
	case (SemanticAction::processAssignment):
		processAssignment();
		break;
	
	case (SemanticAction::processVariableUse):
		processVariableUse();
		break;
	case (SemanticAction::processExpression): 
		processExpression();
		break;
	case (SemanticAction::processTerm):
		processTerm();
		break;
	case (SemanticAction::processIdNestListIdThenIndiceListOrAParams):
		processIdNestListIdThenIndiceListOrAParams();
		break;
	case (SemanticAction::processNum):
		processNum();
		break;
	case(SemanticAction::processIndiceList):
		processIndiceList();
		break;
	case (SemanticAction::processArithExpr):
		processArithExpr();
		break;
	case (SemanticAction::processRelExpr):
		processRelExpr();
		break;
	case (SemanticAction::createSemanticVariableAndLeaveOnStack):
		createSemanticVariableAndLeaveOnStack();
		break;
	case (SemanticAction::processFunctionUse):
		processFunctionUse();
		break;
	}
	//semanticStack.push_back(action);
}

bool Parser::processFunctionUse() {
	GSymbol* symbol;
	if (!(semanticStack.empty())) {
		symbol = semanticStack.back();
	}
	else return false;

	if (symbol->getSymbolType() != GSymbol::terminal) {
		if (insideFinalPass) {
			std::cout << "\nExpected terminal";
		}
		return false;
	}
	GTerminal* idTerm = static_cast<GTerminal*>(symbol);

	if (idTerm->getType() != GTerminal::ID) {
		if (insideFinalPass) {
			std::cout << "\nExpected ID";
		}
		return false;
	}

	if (insideFinalPass) {
		SemanticRecord* ptr = NULL;
		SemanticRecord** rec = &ptr;
		bool b = false;
		bool& found = b;
		currentScope->search(idTerm->getValue(), rec, found);
		if (!found) {

			Logger::getLogger()->log(Logger::SEMANTIC_ERROR, "\nFunction " + idTerm->getValue() + " at line " + std::to_string(idTerm->getPosition().first) + " is not defined in the current scope(" + currentScope->getTableName() + ")");

			error = true;
			return false;
		}
	}

	return true;
}

bool Parser::processVariableUse() {
	GSymbol* symbol;
	if (!(semanticStack.empty())) {
		symbol = semanticStack.back();
	}
	else return false;

	if (symbol->getSymbolType() != GSymbol::terminal) {
		if (insideFinalPass) {
			std::cout << "\nExpected terminal";
		}
		return false;
	}
	GTerminal* idTerm = static_cast<GTerminal*>(symbol);

	if (idTerm->getType() != GTerminal::ID) {
		if (insideFinalPass) {
			std::cout << "\nExpected ID";
		}
		return false;
	}
	
	SemanticRecord* ptr = NULL;
	SemanticRecord** rec = &ptr;
	bool b = false;
	bool& found = b;
	currentScope->search(idTerm->getValue(), rec, found);
	if (!found) {
		// at this point, the variable might not have been defined, but maybe it's defined later
		if (!insideFinalPass) {
			Logger::getLogger()->log(Logger::SEMANTIC_ERROR, "\nIdentifier " + idTerm->getValue() + " at line " + std::to_string(idTerm->getPosition().first) + " is not defined in the current scope(" + currentScope->getTableName() + ")");
		}
		error = true;
		return false;
	}
}

bool Parser::processAssignment() {

	GSymbol* symbol;

	// get the term type to be assigned
	if (!(semanticStack.empty())) {
		symbol = semanticStack.back();
	}
	else return false;

	SemanticRecord::SemanticRecordType termType;
	if (symbol->getSymbolType() != GSymbol::semanticRecord) {
		if (insideFinalPass) {
			std::cout << "\nExpected expr";
		}		
		return false;
	}
	SemanticRecordHolder* typeHolder = static_cast<SemanticRecordHolder*>(symbol);
	termType = typeHolder->getRecord()->getSemanticType();

	// get assignOp
	semanticStack.pop_back();
	if (!(semanticStack.empty())) {
		symbol = semanticStack.back();
	}
	else return false;

	if (symbol->getSymbolType() != GSymbol::terminal) {
		if (insideFinalPass) {
			std::cout << "\nExpected terminal";
		}
		return false;
	}
	GTerminal* assignOp = static_cast<GTerminal*>(symbol);

	if (assignOp->getType() != GTerminal::ASSIGN) {
		if (insideFinalPass) {
			std::cout << "\nExpected assignOp";
		}
		return false;
	}

	// get id
	semanticStack.pop_back();
	if (!(semanticStack.empty())) {
		symbol = semanticStack.back();
	}
	else return false;

	if (symbol->getSymbolType() != GSymbol::terminal) {
		if (insideFinalPass) {
			std::cout << "\nExpected terminal";
		}
		return false;
	}
	GTerminal* idTerm = static_cast<GTerminal*>(symbol);

	if (idTerm->getType() != GTerminal::ID) {
		if (insideFinalPass) {
			std::cout << "\nExpected ID";
		}
		return false;
	}
	SemanticRecord* ptr = NULL;
	SemanticRecord** rec = &ptr;
	bool b = false;
	bool& found = b;
	currentScope->search(idTerm->getValue(), rec, found);
	if (!found) {
		// at this point, the variable might not have been defined, but maybe it's defined later
		if (insideFinalPass) {
			Logger::getLogger()->log(Logger::SEMANTIC_ERROR, "\nIdentifier " + idTerm->getValue() + " at line " + std::to_string(idTerm->getPosition().first) + " is not defined in the current scope(" + currentScope->getTableName() + ")");
		}
		error = true;
		return false;
		
	}
	if ((*rec)->getSemanticType() != termType) {
		if (insideFinalPass) {
			Logger::getLogger()->log(Logger::SEMANTIC_ERROR, "\nType mismatch: right side of assign statement does not match the type of left side, on line " + idTerm->getPosition().first);
		}
		error = true;
		return false;
	}	

	// clean up the id from the stack
	semanticStack.pop_back();

}

void Parser::searchInScope(SymbolTable* scope, std::string identifier, SemanticRecord** record, bool &found) {
	scope->search(identifier, record, found);
	if (!found) {
		SemanticRecord* ptr = NULL;
		SemanticRecord** func = &ptr;
		bool b = false;
		bool& foundFunc = b;
		if (scope->getParent() != NULL) {
			scope->getParent()->search(scope->getTableName(), func, foundFunc);
			if (!foundFunc || !func || (*func)->getSemanticType() != SemanticRecord::FUNCTION) {
				std::cout << "\nsomething really wrong happened here";
				return;
			}
			SemanticFunction* myFunc = static_cast<SemanticFunction*>(*func);			
			if (myFunc->hasParam(identifier)) {
				found = true;
				*record = myFunc->getParam(identifier);
			}
		}

	}
}

bool Parser::processExpression() {
	//GTerminal::TerminalTypes operations[9] = { GTerminal::GT, GTerminal::GE, GTerminal::LE, GTerminal::LT, GTerminal::EQ, GTerminal::NE, GTerminal::PLUS, GTerminal::MINUS, GTerminal::OR };
	std::list<GTerminal::TerminalTypes> operationsForProcessExpr;
	operationsForProcessExpr.emplace_back(GTerminal::GT);
	operationsForProcessExpr.emplace_back(GTerminal::GE);
	operationsForProcessExpr.emplace_back(GTerminal::LE);
	operationsForProcessExpr.emplace_back(GTerminal::LT);
	operationsForProcessExpr.emplace_back(GTerminal::EQ);
	operationsForProcessExpr.emplace_back(GTerminal::NE);
	operationsForProcessExpr.emplace_back(GTerminal::PLUS);
	operationsForProcessExpr.emplace_back(GTerminal::MINUS);
	operationsForProcessExpr.emplace_back(GTerminal::OR);

	return processOperation(operationsForProcessExpr);
}

bool Parser::processRelExpr() {
	std::list<GTerminal::TerminalTypes> operationsForRelExpr;
	operationsForRelExpr.emplace_back(GTerminal::GT);
	operationsForRelExpr.emplace_back(GTerminal::GE);
	operationsForRelExpr.emplace_back(GTerminal::LE);
	operationsForRelExpr.emplace_back(GTerminal::LT);
	operationsForRelExpr.emplace_back(GTerminal::EQ);
	operationsForRelExpr.emplace_back(GTerminal::NE);

	bool success = processOperation(operationsForRelExpr);

	// upon successful parsing of operation, it will push the type holder back on semantic stack
	// in the case of a relation expression, we no longer need it, so remove it
	if (success) {
		semanticStack.pop_back();
	}

	return success;
}

// I assume that terms have already been processed
// term is of form: term (addOp term)*
bool Parser::processArithExpr() {
	std::list<GTerminal::TerminalTypes> operations;
	operations.emplace_back(GTerminal::PLUS);
	operations.emplace_back(GTerminal::MINUS);
	operations.emplace_back(GTerminal::OR);
	return processOperation(operations);
}

// I assume that factors have already been processed
// term is of form: factor (multOp factor)*
bool Parser::processTerm() {
	std::list<GTerminal::TerminalTypes> operations;
	operations.emplace_back(GTerminal::MULT);
	operations.emplace_back(GTerminal::DIVIDE);
	operations.emplace_back(GTerminal::AND);
	return processOperation(operations);
}

// I assume that types have already been processed
// term is of form: type (op type)*
bool Parser::processOperation( const std::list<GTerminal::TerminalTypes> operations) {

	GSymbol* symbol; 

	// get first factor
	if (!(semanticStack.empty())) {
		symbol = semanticStack.back();
	}
	else return false;

	SemanticRecord::SemanticRecordType termType;
	if (symbol->getSymbolType() != GSymbol::semanticRecord) {
		if (insideFinalPass) {
			std::cout << "\nExpected factor";
		}
		return false;
	}
	SemanticRecordHolder* typeHolder = static_cast<SemanticRecordHolder*>(symbol);
	termType = typeHolder->getRecord()->getSemanticType();

	semanticStack.pop_back();
	if (!(semanticStack.empty())) {
		symbol = semanticStack.back();
	}
	// maybe there was just 1 factor or term and nothing else, in that case
	// push it back on the semantic stack, because we just deleted it, and leave it there
	else {
		semanticStack.push_back(typeHolder);
		return true;
	}
	
	while (true) {
		bool shouldBreak = true;
		//for (int i = 0; i < sizeof(operations); ++i) {
			//GTerminal::TerminalTypes operation = operations[i];
			//std::cout << "\n" << GTerminal::getTerminalTypeStringNoQuotes(static_cast<int>(operation));
		//}
		for (GTerminal::TerminalTypes operation : operations) {
			if(symbol->getSymbolType() == GSymbol::terminal && static_cast<GTerminal*>(symbol)->getType() == operation){
				shouldBreak = false;
			}
		}
		if (shouldBreak) break;

		// get multOp
		if (symbol->getSymbolType() != GSymbol::terminal) {
			if (insideFinalPass) {
				std::cout << "\nExpected terminal";
			}
			return false;
		}
		GTerminal* multOp = static_cast<GTerminal*>(symbol);

		bool foundOp = false;
		for (GTerminal::TerminalTypes operation : operations) {
			if (multOp->getType() == operation) {
				foundOp = true;
			}
		}
		if (!foundOp) {
			if (insideFinalPass) {
				std::cout << "\nExpected an operation";
			}
			return false;
		}
		semanticStack.pop_back();
		if (!(semanticStack.empty())) {
			symbol = semanticStack.back();
		}
		else return false;

		// get next factor
		if (symbol->getSymbolType() != GSymbol::semanticRecord) {
			if (insideFinalPass) {
				std::cout << "\nExpected semanticRecord";
			}
			return false;
		}
		SemanticRecordHolder* recHolder = static_cast<SemanticRecordHolder*>(symbol);
		
		// check type

		SemanticRecord::SemanticRecordType t = recHolder->getRecord()->getSemanticType();
		if (t != termType) {
			if (insideFinalPass) {
				std::cout << "Type mismatch: " << SemanticRecord::typeStrings[t] << " and " << SemanticRecord::typeStrings[termType]
					<< " at line " << multOp->getPosition().first << ", column " << multOp->getPosition().second;
			}
			return false;
		}

		semanticStack.pop_back();
		if (!(semanticStack.empty())) {
			symbol = semanticStack.back();
		}
		else break;
	}

	semanticStack.push_back(typeHolder);
}


bool Parser::processIdNestListIdThenIndiceListOrAParams() {
	SemanticFunction* funcRecord = NULL;
	SemanticVariable* idRecord = NULL;
	SemanticRecord* pt= NULL;
	SemanticRecord** record = &pt;
	GSymbol* symbol;
	bool b = false;
	bool &found = b;
	bool allGood = false;

	int threshold = 15; // max num of tokens as params
	int count = 0;
	GTerminal* term = getNextTerminalFromSemanticStack();
	if (term == NULL) { return false; }

	// if there's a closepar, then we have an aParams list
	if (term->getType() == GTerminal::CLOSEPAR) {
		while (term->getType() != GTerminal::OPENPAR && ++count < threshold) {
			
			// pop closepar
			semanticStack.pop_back();
			
			// here we can have either a terminal or a type holder
			symbol = semanticStack.back();
			if (symbol == NULL) { return false; }			
			if (symbol->getSymbolType() == GSymbol::terminal && static_cast<GTerminal*>(symbol)->getType() == GTerminal::OPENPAR) { 
				term = static_cast<GTerminal*>(symbol);
				continue; 
			}
			if (symbol->getSymbolType() != GSymbol::semanticRecord && symbol->getSymbolType() != GSymbol::terminal) {
				return false;
			}

			// pop the terminal or type holder
			// TODO verify params
			semanticStack.pop_back();

			// now we are supposed to have a comma
			term = getNextTerminalFromSemanticStack();
			if (term == NULL) { return false; }
		}
		if (count >= threshold) {
			std::cout << "Something went wrong while doing semantic analysis of aParams! Everything else will probably break from now on.";
		}

		// since count did not exceed threshold, the only way we can get here 
		// is if term is an openpar, and we need to pop it.
		semanticStack.pop_back();
		
		// this will be the function name! keep it
		term = getNextTerminalFromSemanticStack();
		if (term == NULL) { return false; }
		GTerminal* functionIdToken = new GTerminal(term);
		semanticStack.pop_back();

		// if no more terminals, or if next terminal is not a dot, then we're in current scope
		term = getNextTerminalFromSemanticStack();
		if (term == NULL || term->getType() != GTerminal::DOT) {	
			currentScope->search(functionIdToken->getValue(), record, found);
			if (!found) {
				// at this point, function might not have been defined, but later it's maybe defined, so wait till second pass
				if (insideFinalPass) {
					//Logger::getLogger()->log(Logger::SEMANTIC_ERROR, "\nIdentifier '" + functionIdToken->getValue() + " at line " + std::to_string(functionIdToken->getPosition().first) + "' is not defined in current scope (" + currentScope->getTableName() + ")");
					error = true;
				}
			}
			semanticStack.pop_back();
		}
		// otherwise we need to figure out the scope
		else {
			if (processIdNestList(record, found)) {
				if (found) {
					if ((*record)->getSemanticStructure() == SemanticRecord::CLASS_S) {
						SymbolTable* table = static_cast<SemanticClass*>(*record)->getLocalSymbolTable();
						table->search(functionIdToken->getValue(), record, found);
						if (found) {							
							if ((*record)->getSemanticType() == SemanticRecord::FUNCTION) {
								allGood = true;
								funcRecord = static_cast<SemanticFunction*>(*record);
							}							
						}
					}
				}
			}			
		}
	}

	// otherwise, we have an id, and then idNestList
	else if (term->getType() == GTerminal::ID) {
		GTerminal* IDToken = new GTerminal(term);
		semanticStack.pop_back();

		// idNestList might be epsilon, so check:
		GTerminal* term = getNextTerminalFromSemanticStack();
		if (term == NULL || term->getType() != GTerminal::DOT) {
			
			searchInScope(currentScope, IDToken->getValue(), record, found);
			if (found) {
				allGood = true;
				idRecord = static_cast<SemanticVariable*>(*record);
			}
			else {
				if (insideFinalPass) {
					Logger::getLogger()->log(Logger::SEMANTIC_ERROR, "\nIdentifier " + IDToken->getValue() + 
						" at line " + std::to_string(IDToken->getPosition().first) + " was not defined in the current scope (" + currentScope->getTableName() + ")" );
				}
				semanticStack.pop_back();
				return false;
			}
		}
		else if (processIdNestList(record, found)) {
			if (found) {
				if ((*record)->getSemanticStructure() == SemanticRecord::CLASS_S) {
					SymbolTable* table = static_cast<SemanticClass*>(*record)->getLocalSymbolTable();
					table->search(IDToken->getValue(), record, found);
					if (found) {
						allGood = true;
					}
					else {
						if (insideFinalPass) {
							Logger::getLogger()->log(Logger::SEMANTIC_ERROR, "\nIdentifier " + IDToken->getValue() + " was not defined in the current scope!");
						}
						semanticStack.pop_back();
						return false;
					}
				}
			}
		}
	}

	else {
		std::cout << "Could not do semantic processing of IdestListIdThenIndiceListOrAParams";
	}

	if (allGood) {
		if (idRecord != NULL) {
			currentType = idRecord->getSemanticType();					
		}
		else if (funcRecord != NULL) {
			currentType = funcRecord->getReturnType()->getSemanticType();
		}
		std::list<int> myList;
		SemanticType* typeRecord = new SemanticType(currentType, UNKNOWN_VALUE, SemanticRecord::SIMPLE, myList, 0, idRecord->getPosition());
		semanticStack.push_back(new SemanticRecordHolder(typeRecord));
		return true;
	}
	return false;
	
}

bool Parser::processIdNestList(SemanticRecord** record, bool &found) {
	GTerminal* term = getNextTerminalFromSemanticStack();
	if (term == NULL) { return false; }

	// pop dot operator 
	if (term->getType() == GTerminal::DOT) {
		semanticStack.pop_back();
		GTerminal* term = getNextTerminalFromSemanticStack();
		if (term == NULL) { return false; }
	}

	// collect scopes
	std::stack<GTerminal*> scopeTokens;
	while (term != NULL) {
		processIndiceList();
		term = getNextTerminalFromSemanticStack();
		if (term == NULL) { return false; }
		if (term->getType() == GTerminal::ID) {
			scopeTokens.push(new GTerminal(term));
		}

		semanticStack.pop_back();
		term = getNextTerminalFromSemanticStack();
		if (term == NULL) { continue; }

		// pop dot operator 
		if (term->getType() == GTerminal::DOT) {
			semanticStack.pop_back();
			GTerminal* term = getNextTerminalFromSemanticStack();
			if (term == NULL) { return false; }
		}
		else {
			if (insideFinalPass) {
				std::cout << "Expected dot operator";
			}
			return false;
		}
		semanticStack.pop_back();
		GTerminal* term = getNextTerminalFromSemanticStack();
	}

	int count = scopeTokens.size();
	while (!scopeTokens.empty()) {
		GTerminal* term = scopeTokens.top(); 
		currentScope->search(term->getValue(), record, found);
		if (found) {
			semanticStack.pop_back();
			semanticStack.push_back(new SemanticRecordHolder(*record));
			scopeIn();			
		}
		else {
			if (insideFinalPass) {
				Logger::getLogger()->log(Logger::SEMANTIC_ERROR, "\nIdentifier " + term->getValue() + " at line " + std::to_string(term->getPosition().first) + " is not defined in the current scope(" + currentScope->getTableName() + ")");
			}
			error = true;
			return false;
		}
		
		scopeTokens.pop();
	}

	for (int i = 0; i < count; ++i) {
		scopeOut();
	}
	return true;
}

void Parser::processIndiceList() {
	currentIndiceDimention = 0;
	GTerminal* term = getNextTerminalFromSemanticStack();
	if (term == NULL) { return; }
	int count = 0;
	GTerminal* lastTerm = term;

	// indiceList can be empty
	if (term->getType() != GTerminal::CLOSESQUARE) {
		return;
	}
	bool intError = false;
	while (term->getType() == GTerminal::CLOSESQUARE) {
		++count;
		semanticStack.pop_back();
		lastTerm = term;		

		// we got ]
		// we now need a number
		// it can be either an INTNUM, or a SemanticRecordHolder of type INT

		GSymbol* symbol = semanticStack.back();
		if (symbol->getSymbolType() != GSymbol::terminal && symbol->getSymbolType() != GSymbol::semanticRecord) {
			intError = true;
			break;
		}
		if (symbol->getSymbolType() == GSymbol::terminal && static_cast<GTerminal*>(symbol)->getType() != GTerminal::INTNUM) {
			intError = true;
			break;			
		}
		else if (symbol->getSymbolType() == GSymbol::semanticRecord && 
			static_cast<SemanticRecordHolder*>(symbol)->getRecord()->getSemanticType() != SemanticRecord::INT) {
			intError = true;
			break;
		}

		// ok, we verified we have an int. now pop it from semantic stack
		semanticStack.pop_back();
		
		// get [
		term = getNextTerminalFromSemanticStack();
		if (term == NULL) { return; }
		if (term->getType() != GTerminal::OPENSQUARE) {
			if (insideFinalPass) {
				std::cout << "Expected ], found something else" << term->getPosition().first << ", " << term->getPosition().second;
			}
			return;
		}
		lastTerm = term;

		// ok, we verified it's a [, now pop it from semantic stack
		semanticStack.pop_back();

		// get the next ] and set it as term
		term = getNextTerminalFromSemanticStack();
		if (term == NULL) { return; }
	} // end while 

	if (intError) {
		semanticError = true;
		//Logger::getLogger()->log(Logger::SEMANTIC_ERROR, "Expected integer as indice, found something else at line " + term->getPosition().first);
		if (insideFinalPass) {
			std::cout << "Expected integer as indice, found something else" << term->getPosition().first;
		}
	}

	if (lastTerm->getType() != GTerminal::OPENSQUARE) {
		if (insideFinalPass) {
			std::cout << "Something went wrong while doing semantic analysis of indiceList! Everything else will probably break from now on.";
		}
	}
	currentIndiceDimention = count / 3;
}

void Parser::processNum() {
	GTerminal* term = getNextTerminalFromSemanticStack();
	if (term == NULL) { return; }
	if (term->getType() == GTerminal::INTNUM) {
		currentType = SemanticRecord::INT;		
	}
	else if (term->getType() == GTerminal::FLOATNUM) {
		currentType = SemanticRecord::FLOAT;
	}
	else if (term->getType() == GTerminal::CLOSESQUARE) {
		processIndiceList();
		term = getNextTerminalFromSemanticStack();
		if (term->getType() == GTerminal::INTNUM) {
			currentType = SemanticRecord::INT;
		}
		else if (term->getType() == GTerminal::FLOATNUM) {
			currentType = SemanticRecord::FLOAT;
		}
	}
	else {
		//std::cout << "Could not process num, the symbol on top of stack is not int nor float";
		return;
	}
	std::list<int> myList;
	SemanticType* typeRecord = new SemanticType(currentType, term->getValue(), SemanticRecord::SIMPLE, myList, 0, term->getPosition());
	semanticStack.pop_back();
	semanticStack.push_back(new SemanticRecordHolder(typeRecord));
}

void Parser::scopeIn() {
	if (!(semanticStack.empty())) {
		GSymbol* symbol = semanticStack.back();
		if (symbol->getSymbolType() == GSymbol::semanticRecord) {
			SemanticRecordHolder* hold = static_cast<SemanticRecordHolder*>(symbol);
			if (hold->getRecord()->getSemanticType() == SemanticRecord::FUNCTION) {
				semanticStack.pop_back();
				currentScope = static_cast<SemanticFunction*>(hold->getRecord())->getLocalSymbolTable();
				if (currentScope == NULL) {
					std::cout << "";
				}
			}
			else if(hold->getRecord()->getSemanticType() == SemanticRecord::CLASS_T) {
				semanticStack.pop_back();
				currentScope = static_cast<SemanticClass*>(hold->getRecord())->getLocalSymbolTable();
				if (currentScope == NULL) {
					std::cout << "";
				}
			}
			else {
				if (insideFinalPass) {
					std::cout << "cannot scope in, top of semantic stack is not a function nor a class";
				}
			}
		}
		else {
			if (insideFinalPass) {
				std::cout << "cannot scope in, top of semantic stack is not a record";
			}
		}		
	}
	else {
		if (insideFinalPass) {
			std::cout << "cannot scope in, semantic stack empty";
		}
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
		if (term->getType() == GTerminal::INTNUM) {
			currentArraySizeList.emplace_back(atoi(term->getValue().c_str()));
		}		
		semanticStack.pop_back();
		term = getNextTerminalFromSemanticStack();
		if (term == NULL) { return false; }
		expectedType = ++expectedType % 3;
	}

	return true;
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
	std::list<int> arrayDimension;
	if (funcRetTypeToken->getType() == GTerminal::INTWORD) { recordStructure = SemanticRecord::SIMPLE; isInt = true; }
	else if (funcRetTypeToken->getType() == GTerminal::FLOATWORD) { recordStructure = SemanticRecord::SIMPLE; }
	else { recordStructure = SemanticRecord::CLASS_S; }
	if (!(currentArraySizeList.empty())) {
		recordStructure = SemanticRecord::ARRAY;
		arrayDimension = std::list<int>(currentArraySizeList);
	}
	SymbolTable* functionTable = new SymbolTable(currentScope, funcIDtoken->getValue());

	SemanticRecord::SemanticRecordType returnType;
	if (funcRetTypeToken->getType() == GTerminal::INTWORD) {
		returnType = SemanticRecord::INT;
	}
	else if (funcRetTypeToken->getType() == GTerminal::FLOATWORD) {
		returnType = SemanticRecord::FLOAT;
	}
	else if (funcRetTypeToken->getType() == GTerminal::ID) {
		returnType = SemanticRecord::CLASS_T;
	}
	else {
		if (insideFinalPass) {
			std::cout << "\nCould not process function return type for function " << funcIDtoken->getValue() << "! ";
		}
		return;
	}
	std::list<int> myList;
	SemanticFunction* funcRecord = new SemanticFunction(funcIDtoken->getValue(), recordStructure, arrayDimension, 0, paramList, functionTable, new SemanticType(returnType, UNKNOWN_VALUE, SemanticRecord::SIMPLE, myList, 0, funcIDtoken->getPosition()), funcIDtoken->getPosition());
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
		std::list<int> myList;
		SemanticClass* classRecord = new SemanticClass(className, myList, 0, classTable, term->getPosition());
		if (!insideFinalPass) {
			currentScope->insert(className, classRecord);
		}		
		currentScope = classTable;
		classRecord->setDeclared();
		semanticStack.pop_back();
	}
	else {
		Logger::getLogger()->log(Logger::SEMANTIC_ERROR, "\nExpected terminal on top of stack, but there is something else! Something went really wrong.");
		std::cout << "Expected terminal on top of stack, but there is something else! Something went really wrong.";
		error = true;
	}
}

void Parser::logSymbolErrorAndSetFlag(std::string symbol) {
	if (insideFinalPass) {
		Logger::getLogger()->log(Logger::SEMANTIC_ERROR, "\nExpected " + symbol + " on top of stack, but there is something else! Something went really wrong.");
		std::cout << "Expected " + symbol + " on top of stack, but there is something else! Something went really wrong.";
	}
	error = true;
}

// used in the for statement, where we declare and initialize a variable in one statement
void Parser::createSemanticVariableAndLeaveOnStack() {

	// get copy of ID token
	GTerminal* term = getNextTerminalFromSemanticStack();
	if (term->getType() != GTerminal::ID) {
		logSymbolErrorAndSetFlag("identifier");
		return;
	}
	GTerminal* varIDToken = new GTerminal(term);

	// create variable
	SemanticVariable* var = createSemanticVariable(false);
	var->setDeclared();
	currentScope->insert(var->getIdentifier(), var);

	// push back the id token, for the assignment statement to run correctly
	semanticStack.push_back(varIDToken);
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


	// if ID, then it's a class. If in final pass, verify if it is declared
	if (insideFinalPass) {
		if (varTypeToken->getType() == GTerminal::ID) {
			SemanticRecord* ptr = NULL;
			SemanticRecord** rec = &ptr;
			bool b = false;
			bool& found = b;
			currentScope->search(varTypeToken->getValue(), rec, found);
			if (!found) {					
				Logger::getLogger()->log(Logger::SEMANTIC_ERROR, "\nIdentifier " + varTypeToken->getValue() + " at line " + std::to_string(varTypeToken->getPosition().first) + " is not defined in the current scope(" + currentScope->getTableName() + ")");
				error = true;
				return false;
			}
		}
	}

	SemanticRecord::SemanticRecordType recordType;
	SemanticRecord::SemanticStructure recordStructure;
	bool isInt = false;
	std::list<int> arrayDimension;
	if (varTypeToken->getType() == GTerminal::INTWORD) { recordType = SemanticRecord::INT; recordStructure = SemanticRecord::SIMPLE; isInt = true; }
	else if (varTypeToken->getType() == GTerminal::FLOATWORD) { recordType = SemanticRecord::FLOAT; recordStructure = SemanticRecord::SIMPLE;}
	else { recordType = SemanticRecord::CLASS_T; recordStructure = SemanticRecord::CLASS_S;	}
	if (!(currentArraySizeList.empty())) {
		recordStructure = SemanticRecord::ARRAY;
		arrayDimension = std::list<int>(currentArraySizeList);
	}
	SemanticVariable::VariableKind varKind;
	if (fParam) {
		varKind = SemanticVariable::PARAM;
	}
	else {
		varKind = SemanticVariable::NORMAL;
	}
	
	SemanticVariable* varRecord = new SemanticVariable(varIDToken->getValue(), recordType, recordStructure, arrayDimension, 0, varKind, varIDToken->getPosition());
	return varRecord;
}



GTerminal* Parser::getNextTerminalFromSemanticStack() {
	if (semanticStack.empty() == true) {
		return NULL;
	}
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
	if (ruleNo == 79 || ruleNo == 80) {
		std::cout << "";
	}

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

			if (rule.back()->getSymbolType() == GSymbol::semanticAction) {
				std::pair<int, int> pos;
				if (lastTerminal == NULL) {
					pos.first = 0;
					pos.second = 0;
				}
				else {
					pos = lastTerminal->getPosition();
				}
				
				SemanticAction* act = static_cast<SemanticAction*>(rule.back());
				act->setPosition(pos);
			}

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

//const std::list<GTerminal::TerminalTypes> Parser::operationsForProcessExpr[9] = { GTerminal::GT, GTerminal::GE, GTerminal::LE, GTerminal::LT, GTerminal::EQ, GTerminal::NE, GTerminal::PLUS, GTerminal::MINUS, GTerminal::OR };