#pragma once
#include "SemanticRecord.h"
#include "SymbolTable.h"

class SemanticClass : public SemanticRecord {
	SymbolTable* localSymbolTable;

public:
	SemanticClass(std::string, int, int, SymbolTable* lSymbols);
	~SemanticClass();

	void printDetail();
	GSymbol * clone() { std::cout << "Clone method not implemented!"; }
	GSymbolTypes getSymbolType() { return GSymbol::semanticRecord; }
	bool isDollarSign() const { return false; }
	bool isEpsilon() const { return false; }
};