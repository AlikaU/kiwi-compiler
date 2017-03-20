#pragma once
#include "SemanticRecord.h"

class SemanticVariable : public SemanticRecord {

public:
	enum VariableKind {NORMAL = 0, PARAM = 1};
	SemanticVariable(std::string, SemanticRecordType, SemanticStructure, int, int, VariableKind);
	std::string VariableKindString[2] = { "NORMAL", "PARAM" };
	void printDetail();
	

private: 
	VariableKind kind; // normal or param
	
};