#include "SemanticClass.h"
#include <iostream>
#include <string>

SemanticClass::SemanticClass(std::string identifier, int arrDimension, int addr, SymbolTable* lSymbols)
	: SemanticRecord(identifier, SemanticRecordType::CLASS_T, SemanticStructure::CLASS_S, arrDimension, addr) {

	localSymbolTable = lSymbols;
}

SemanticClass::~SemanticClass() {
	delete localSymbolTable;
}

void SemanticClass::printDetail() {

	if (localSymbolTable) {
		std::cout << ", local symbol table: \n\t";
		localSymbolTable->print();
	}
	else std::cout << ", no local symbol table.";
}