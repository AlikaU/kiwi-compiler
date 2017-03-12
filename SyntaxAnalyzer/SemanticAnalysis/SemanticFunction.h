#pragma once
#include "SemanticRecord.h"
#include "SymbolTable.h"

class SemanticFunction : SemanticRecord {

	int numOfParams;
	std::list<SemanticRecordType> params;

	SymbolTable* localSymbols;

public: 
	SemanticRecordType getType() { return SemanticRecordType::FUNCTION; }
	void printDetail();
};