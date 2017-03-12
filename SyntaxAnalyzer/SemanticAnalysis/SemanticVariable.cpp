#include "SemanticVariable.h"
#include <iostream>
#include <string>

void SemanticVariable::printDetail() {
	std::string typeStr = "";
	if (isInt) {
		typeStr = "int";
	}
	else typeStr = "float";
	std::cout << ", " << typeStr << ", " << VariableKindString[static_cast<int>(kind)];
}