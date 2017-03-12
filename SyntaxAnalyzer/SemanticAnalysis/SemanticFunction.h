#pragma once
#include "SemanticRecord.h"
#include "SymbolTable.h"

class SemanticFunction : SemanticRecord {

	
	int numOfParams;
	std::list<SemanticRecordType>* params;

	SymbolTable* localSymbolTable;

public: 
	SemanticFunction(std::string, SemanticRecordType, SemanticStructure, int, int,
		std::list<SemanticRecordType>* prms, SymbolTable* lSymbols);
	~SemanticFunction();
	SemanticRecordType getType() { return SemanticRecordType::FUNCTION; }
	void printDetail();
};