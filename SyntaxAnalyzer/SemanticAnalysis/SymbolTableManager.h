#pragma once
#include "SymbolTable.h"
#include "SemanticRecord.h"
#include <list>

class SymbolTableManager {

	

public:
	static SymbolTable* createTable(std::string);
	static SymbolTable* createTable(SymbolTable*, std::string);
	static void search(SymbolTable*, std::string identifier, SemanticRecord*, bool*);
	static void insert(SymbolTable*, std::string identifier, SemanticRecord*);
	static void deleteTable(SymbolTable*);
	static void printTable(SymbolTable*);


};