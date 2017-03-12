#include "SemanticFunction.h"
#include <iostream>
#include <string>

void SemanticFunction::printDetail() {
	std::cout << ", " << numOfParams << " parameters: {";
	for (SemanticRecordType t : params) {
		std::cout << typeStrings[static_cast<int>(t)] << ", ";
	}
	std::cout << "}";

	if (localSymbols) {
		std::cout << "local symbol table: \n\t";
		localSymbols->print();
	}
	else std::cout << "no local symbol table.";
}