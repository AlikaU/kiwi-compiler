#include "SymbolTable.h"
#include <iostream>
#include <string>

void SymbolTable::search(std::string identifier, SemanticRecord** ptr, bool &found) {
	for (SemanticRecord* record : *records) {
		if (record->getIdentifier() == identifier) {
			*ptr = record;
			found = true;
			break;
		}
	}
	if (!found && parent != NULL) {
		parent->search(identifier, ptr, found);
	}
}

void SymbolTable::insert(std::string identifier, SemanticRecord* ptr) {
	for (SemanticRecord* record : *records) {
		if (record->getIdentifier() == identifier) {
			std::cout << "\nCannot insert record with identifier " << identifier << ". Another record with the same name exists.";
			return;
		}
	}

	records->push_back(ptr);
}

void SymbolTable::print() {
	for (SemanticRecord* record : *records) {
		record->print();
	}
}