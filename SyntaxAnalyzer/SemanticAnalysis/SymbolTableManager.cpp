#include "SymbolTableManager.h"

SymbolTable* SymbolTableManager::createTable(std::string name) {
	return new SymbolTable(name);
}

SymbolTable* SymbolTableManager::createTable(SymbolTable* ptr, std::string name) {
	return new SymbolTable(ptr, name);
}

void SymbolTableManager::search(SymbolTable* table, std::string identifier, SemanticRecord** ptr, bool &found) {
	table->search(identifier, ptr, found);
}

void SymbolTableManager::insert(SymbolTable* table, std::string identifier, SemanticRecord* ptr) {
	table->insert(identifier, ptr);
}

void SymbolTableManager::deleteTable(SymbolTable* table) {
	table->clearTable();
	delete table;
}

void SymbolTableManager::printTable(SymbolTable* table) {
	table->print();
}