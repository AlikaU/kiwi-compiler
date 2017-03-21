#pragma once
#include "SemanticRecord.h"
#include "SemanticVariable.h"

class SemanticType : public SemanticRecord {
public: 
	SemanticType(SemanticRecordType sType, SemanticStructure sStructure, int aDimension, int addr) : SemanticRecord("Type placeholder ", sType, sStructure, aDimension, addr) {}
	
	void printDetail() {
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
		std::cout << "Type placeholder: " << typeStr;
	}
};