#include "SemanticClass.h"
#include <iostream>
#include <string>

SemanticClass::SemanticClass(std::string identifier, SemanticRecordType sType,
	SemanticStructure sStruct, int arrDimension, int addr, SymbolTable* lSymbols)
	: SemanticRecord(identifier, sType, sStruct, arrDimension, addr) {

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