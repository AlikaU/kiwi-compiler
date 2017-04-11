#include "SemanticVariable.h"
#include <string>

SemanticVariable::SemanticVariable(std::string identifier, SemanticRecordType sType,
	SemanticStructure sStruct, std::list<int> arrDimension, int addr, VariableKind k)
		: SemanticRecord (identifier, sType, sStruct, arrDimension, addr) {
	kind = k;
}

void SemanticVariable::printDetail() {
	std::string typeStr = "";
	if (semanticType == SemanticRecordType::INT) {
		typeStr = "int";
	}
	else if (semanticType == SemanticRecordType::FLOAT) {
		typeStr = "float";
	}
	else if (semanticType == SemanticRecordType::CLASS_T) {
		typeStr = "class";		
	}
	else {
		typeStr = "wrongVariableType";
	}
	Logger::getLogger()->log(Logger::TABLE, ", " + typeStr + ", " + VariableKindString[static_cast<int>(kind)]);
}