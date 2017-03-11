#pragma once
#include "SymbolTable.h"
#include "SemanticRecord.h"
#include <list>

class SymbolTableManager {

	

public:
	static SymbolTable* createTable();
	static SymbolTable* createTable(SymbolTable*);
	static void search(SymbolTable*, std::string identifier, SemanticRecord*, bool*);
	static void insert(SymbolTable*, std::string identifier, SemanticRecord*);
	static void deleteTable(SymbolTable*);
	static void printTable(SymbolTable*);


};