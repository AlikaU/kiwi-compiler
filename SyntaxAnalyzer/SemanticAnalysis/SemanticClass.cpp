#include "SemanticClass.h"
#include <iostream>
#include <string>

void SemanticClass::printDetail() {

	if (localSymbols) {
		std::cout << ", local symbol table: \n\t";
		localSymbols->print();
	}
	else std::cout << ", no local symbol table.";
}