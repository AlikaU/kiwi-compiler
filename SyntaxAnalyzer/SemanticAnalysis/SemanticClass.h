#pragma once
#include "SemanticRecord.h"
#include "SymbolTable.h"

class SemanticClass : public SemanticRecord {
	SymbolTable* localSymbolTable;

public:
	SemanticClass(std::string, std::list<int>, int, SymbolTable* lSymbols, std::pair<int,int>);
	~SemanticClass();
	SymbolTable* getLocalSymbolTable() { return localSymbolTable; }
	void printDetail();

};