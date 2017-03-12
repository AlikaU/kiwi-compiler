#include "SemanticVariable.h"
#include <iostream>
#include <string>

SemanticVariable::SemanticVariable(std::string identifier, SemanticRecordType sType,
	SemanticStructure sStruct, int arrDimension, int addr, VariableKind k, bool ii) 
		: SemanticRecord (identifier, sType, sStruct, arrDimension, addr) {
	kind = k;
	isInt = ii;
}

void SemanticVariable::printDetail() {
	std::string typeStr = "";
	if (isInt) {
		typeStr = "int";
	}
	else typeStr = "float";
	std::cout << ", " << typeStr << ", " << VariableKindString[static_cast<int>(kind)];
}