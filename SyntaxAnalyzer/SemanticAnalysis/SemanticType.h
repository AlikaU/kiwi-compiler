#pragma once
#include "SemanticRecord.h"
#include "SemanticVariable.h"

class SemanticType : public SemanticRecord {

	std::string value;

public: 
	SemanticType(SemanticRecordType sType, std::string v, SemanticStructure sStructure, std::list<int>* aDimension, int addr)
		: SemanticRecord("Type placeholder ", sType, sStructure, aDimension, addr) {
		value = v;
	}

	void printDetail() {
		std::string typeStr = "";
		std::string valStr = "";
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
		Logger::getLogger()->log(Logger::TABLE, "Type placeholder: " + typeStr + ", value: " + value);
	}
};