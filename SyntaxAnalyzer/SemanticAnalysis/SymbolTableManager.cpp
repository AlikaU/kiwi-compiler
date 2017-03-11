#include "SymbolTableManager.h"

SymbolTable* SymbolTableManager::createTable() {
	return new SymbolTable();
}

SymbolTable* SymbolTableManagercreateTable(SymbolTable* ptr) {
	return new SymbolTable(ptr);
}

void SymbolTableManager::search(SymbolTable* table, std::string identifier, SemanticRecord* ptr, bool* found) {
	table->search(identifier, ptr, found);
}

void SymbolTableManagerinsert(SymbolTable* table, std::string identifier, SemanticRecord* ptr) {
	table->insert(identifier, ptr);
}

void SymbolTableManager::deleteTable(SymbolTable* table) {
	table->clear();
	delete table;
}

void printTable(SymbolTable* table) {
	table->print();
}