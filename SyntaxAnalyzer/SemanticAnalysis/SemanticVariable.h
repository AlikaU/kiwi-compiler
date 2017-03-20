#pragma once
#include "SemanticRecord.h"

class SemanticVariable : public SemanticRecord {

public:
	enum VariableKind {NORMAL = 0, PARAM = 1};
	SemanticVariable(std::string, SemanticRecordType, SemanticStructure, int, int, VariableKind);
	std::string VariableKindString[2] = { "NORMAL", "PARAM" };
	void printDetail();
	GSymbol * clone() { std::cout << "Clone method not implemented!"; }
	GSymbolTypes getSymbolType() { return GSymbol::semanticRecord; }
	bool isDollarSign() const { return false; }
	bool isEpsilon() const { return false; }

private: 
	VariableKind kind; // normal or param
	
};