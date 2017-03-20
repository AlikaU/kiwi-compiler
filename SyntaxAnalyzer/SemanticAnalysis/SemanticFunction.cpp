#include "SemanticFunction.h"
#include <iostream>
#include <string>

SemanticFunction::SemanticFunction(std::string identifier,
	SemanticStructure sStruct, int arrDimension, int addr, 
	std::list<SemanticRecordType>* prms, SymbolTable* lSymbols)
	: SemanticRecord(identifier, SemanticRecord::FUNCTION, sStruct, arrDimension, addr) {
	params = prms;
	int count = 0;
	for (SemanticRecordType t : *params) {
		++count;
	}
	numOfParams = count;
	localSymbolTable = lSymbols;
}

SemanticFunction::~SemanticFunction() {
	params->clear();
	delete params;
	delete localSymbolTable;
}

void SemanticFunction::printDetail() {
	std::cout << ", " << numOfParams << " parameters: {";
	for (SemanticRecordType t : *params) {
		std::cout << typeStrings[static_cast<int>(t)] << ", ";
	}
	std::cout << "}";

	if (localSymbolTable) {
		std::cout << "local symbol table: \n\t";
		localSymbolTable->print();
	}
	else std::cout << "no local symbol table.";
}