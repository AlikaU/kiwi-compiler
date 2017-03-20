#pragma once
#include "SemanticRecord.h"
#include "SymbolTable.h"

class SemanticFunction : public SemanticRecord {

	
	int numOfParams;
	std::list<SemanticRecordType>* params;

	SymbolTable* localSymbolTable;

public: 
	SemanticFunction(std::string, SemanticStructure, int, int,
		std::list<SemanticRecordType>* prms, SymbolTable* lSymbols);
	~SemanticFunction();
	SemanticRecordType getType() { return SemanticRecordType::FUNCTION; }
	void printDetail();
};