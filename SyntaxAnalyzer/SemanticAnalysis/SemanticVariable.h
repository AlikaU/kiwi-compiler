#pragma once
#include "SemanticRecord.h"

class SemanticVariable : SemanticRecord {
public:
	enum VariableKind {NORMAL = 0, PARAM = 1};
	SemanticRecordType getType() { if (isInt) return SemanticRecordType::INT; else return SemanticRecordType::FLOAT;}

private: 
	VariableKind kind; // normal or param
	bool isInt; // can be int or float
};