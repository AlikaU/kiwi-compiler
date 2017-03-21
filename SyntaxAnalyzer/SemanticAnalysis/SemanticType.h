#pragma once
#include "SemanticRecord.h"
#include "SemanticVariable.h"

class SemanticType : public SemanticRecord {
	
	SemanticType(SemanticRecordType sType, SemanticStructure sStructure, int aDimension, int addr) : SemanticRecord("temp", sType, sStructure, aDimension, addr) {}
};