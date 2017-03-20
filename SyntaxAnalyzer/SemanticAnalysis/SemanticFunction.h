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
	SemanticRecordType getType() { return SemanticRecordType::FUNCTION; }
	void printDetail();
};