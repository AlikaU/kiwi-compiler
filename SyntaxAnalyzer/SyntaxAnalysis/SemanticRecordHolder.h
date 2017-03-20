#pragma once
#include <iostream>
#include "..\SemanticAnalysis\SemanticRecord.h"

class SemanticRecordHolder : public GSymbol {
public:
	SemanticRecordHolder(SemanticRecord* r) {
		record = r;
	};
	SemanticRecord* getRecord() {
		return record;
	};
	~SemanticRecordHolder() { delete record; };
	GSymbol * clone() const { std::cout << "\nSemanticRecordHolder: clone method not implemented"; return NULL; };
	GSymbolTypes getSymbolType() { return GSymbol::semanticRecord; };
	bool isDollarSign() const { return false; };
	bool isEpsilon() const { return false; };

private:
	SemanticRecord* record;
};