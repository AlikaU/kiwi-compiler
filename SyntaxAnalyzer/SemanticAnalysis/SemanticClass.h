#pragma once
#include "SemanticRecord.h"
#include "SymbolTable.h"

class SemanticClass : SemanticRecord {
	SymbolTable* localSymbols;

public:
	SemanticRecordType getType() {return SemanticRecordType::CLASS;}

};