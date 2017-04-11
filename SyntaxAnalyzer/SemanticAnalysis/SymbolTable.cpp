#include "SymbolTable.h"
#include <iostream>
#include <string>

void SymbolTable::search(std::string identifier, SemanticRecord** ptr, bool &found) {
	if (records != NULL) {
		for (SemanticRecord* record : *records) {
			if (record->getIdentifier() == identifier) {
				*ptr = record;
				found = true;
				break;
			}
		}		
	}
	if (!found && parent != NULL) {
		parent->search(identifier, ptr, found);
	}
}

bool SymbolTable::insert(std::string identifier, SemanticRecord* ptr) {
	for (SemanticRecord* record : *records) {
		if (record->getIdentifier() == identifier) {
			Logger::getLogger()->log(Logger::SEMANTIC_ERROR, "\nCannot insert record with identifier " + 
				identifier + " at line " + std::to_string(record->getPosition().first) + 
				", column " + std::to_string(record->getPosition().second) +
				". Another record with the same name exists.");
			return false;;
		}
	}

	records->push_back(ptr);
	return true;
}

void SymbolTable::print() {
	for (SemanticRecord* record : *records) {
		record->print();
	}
}