#pragma once
#include "SemanticRecord.h"
#include "SemanticVariable.h"

class SemanticType : public SemanticRecord {
	
	SemanticType(SemanticVariable* var) : SemanticRecord(var->getIdentifier(), var->getSemanticType(), var->getSemanticStructure(), var->getArrayDimension(), var->getAddress()) {

	}
};