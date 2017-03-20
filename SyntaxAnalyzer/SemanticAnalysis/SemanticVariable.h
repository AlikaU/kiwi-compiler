#pragma once
#include "SemanticRecord.h"

class SemanticVariable : public SemanticRecord {

public:
	enum VariableKind {NORMAL = 0, PARAM = 1};
	SemanticVariable(std::string, SemanticRecordType, SemanticStructure, int, int, VariableKind, bool);
	std::string VariableKindString[2] = { "NORMAL", "PARAM" };
	SemanticRecordType getType() { if (isInt) return SemanticRecordType::INT; else return SemanticRecordType::FLOAT;}
	void printDetail();

private: 
	VariableKind kind; // normal or param
	bool isInt; // can be int or float
	
};