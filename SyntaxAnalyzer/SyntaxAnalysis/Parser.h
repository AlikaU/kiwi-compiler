#pragma once
#include "..\..\Project1\Scanner.h"
#include "GSymbol.h"
#include "GTerminal.h"
#include "GNonTerminal.h"
#include "..\..\Project1\Token.h"
#include "ParseTable.h"
#include <stack>
#include <deque>
#include "SemanticAction.h"
#include "..\SemanticAnalysis\SemanticClass.h"
#include "..\SemanticAnalysis\SemanticFunction.h"
#include "..\SemanticAnalysis\SemanticRecord.h"
#include "..\SemanticAnalysis\SemanticVariable.h"
#include "SemanticRecordHolder.h"
#include "..\SemanticAnalysis\SemanticType.h"

#include "..\SemanticAnalysis\SymbolTable.h"

#define GLOBAL_TABLE_NAME "GlobalSymbolTable"

class Parser {

	Scanner* scanner;
	ParseTable* table;
	std::stack<GSymbol*> parsingStack;
	std::deque<GSymbol*> semanticStack;
	bool error;
	void inverseRHSMultiplePush(int ruleNo);
	std::list<GSymbol*> derivationParsed;
	std::list<GSymbol*> derivationToBeParsed;
	void printDerivation();
	void printDerivationToConsole();
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
	bool printDerivToConsole;
	void printSemanticStack();
	void processSemanticAction(SemanticAction* action);
	bool isCollecting;
	SymbolTable* globalSymbolTable;
	SymbolTable* currentScope;
	std::list<GTerminal*> currentArraySizeList;
	SemanticVariable* createSemanticVariable(bool);
	void createSemanticClassAndTable();
	void createSemanticFunctionAndTable();
	bool processArraySizeList();
	void logSymbolErrorAndSetFlag(std::string);
	GTerminal* getNextTerminalFromSemanticStack();
	void scopeIn();
	void scopeOut();
	bool processAssignment();
	SemanticRecord::SemanticRecordType currentType;
	void processNum();

public: 
	Parser(Scanner* s, ParseTable* t, bool p, bool c);
	~Parser();	
		
	bool parse();
	void skipErrors(int errorCode);
};