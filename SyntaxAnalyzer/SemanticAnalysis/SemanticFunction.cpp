#include "SemanticFunction.h"
#include <iostream>
#include <string>

SemanticFunction::SemanticFunction(std::string identifier,
	SemanticStructure sStruct, int arrDimension, int addr, 
	std::list<SemanticVariable*>* prms, SymbolTable* lSymbols, SemanticType* rType)
	: SemanticRecord(identifier, SemanticRecord::FUNCTION, sStruct, arrDimension, addr) {
	params = prms;
	returnType = rType;
	int count = 0;
	for (SemanticVariable* var : *params) {
		++count;
	}
	numOfParams = count;
	localSymbolTable = lSymbols;
}

SemanticFunction::~SemanticFunction() {
	params->clear();
	delete returnType;
	delete params;
	delete localSymbolTable;
}

void SemanticFunction::printDetail() {
	std::cout << ", " << numOfParams << " parameters: {";
	for (SemanticVariable* var : *params) {
		std::cout << typeStrings[static_cast<int>(var->getSemanticType())] << ", ";
	}
	std::cout << "}";

	if (localSymbolTable) {
		std::cout << "local symbol table: \n\t";
		localSymbolTable->print();
	}
	else std::cout << "no local symbol table.";
}