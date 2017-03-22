#pragma once
#include "SemanticRecord.h"
#include "SymbolTable.h"
#include "SemanticVariable.h"
#include "SemanticType.h"

class SemanticFunction : public SemanticRecord {

	
	int numOfParams;
	std::list<SemanticVariable*>* params;
	SemanticType* returnType;
	SymbolTable* localSymbolTable;

public: 
	SemanticFunction(std::string, SemanticStructure, int, int,
		std::list<SemanticVariable*>* prms, SymbolTable* lSymbols, SemanticType* rType);
	~SemanticFunction();
	SymbolTable* getLocalSymbolTable() { return localSymbolTable; }
	void printDetail();
	SemanticType* getReturnType() const { return returnType; }

};