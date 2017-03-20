#pragma once
#include "SemanticRecord.h"
#include "SymbolTable.h"
#include "SemanticVariable.h"

class SemanticFunction : public SemanticRecord {

	
	int numOfParams;
	std::list<SemanticVariable*>* params;

	SymbolTable* localSymbolTable;

public: 
	SemanticFunction(std::string, SemanticStructure, int, int,
		std::list<SemanticVariable*>* prms, SymbolTable* lSymbols);
	~SemanticFunction();
	void printDetail();
	GSymbol * clone() { std::cout << "Clone method not implemented!"; }
	GSymbolTypes getSymbolType() { return GSymbol::semanticRecord; }
	bool isDollarSign() const { return false; }
	bool isEpsilon() const { return false; }
};