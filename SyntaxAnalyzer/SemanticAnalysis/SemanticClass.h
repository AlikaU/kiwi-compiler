#pragma once
#include "SemanticRecord.h"
#include "SymbolTable.h"

class SemanticClass : public SemanticRecord {
	SymbolTable* localSymbolTable;

public:
	SemanticClass(std::string, int, int, SymbolTable* lSymbols);
	~SemanticClass();
	SymbolTable* getLocalSymbolTable() { return localSymbolTable; }
	void printDetail();

};