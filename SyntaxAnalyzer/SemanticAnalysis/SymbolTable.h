#pragma once
#include "SemanticRecord.h"
#include <list>

class SymbolTable {

	SymbolTable* parent;
	std::list<SemanticRecord*>* records;

public: 

	SymbolTable() {}
	SymbolTable(SymbolTable* p) { parent = p; records = new std::list<SemanticRecord*>; }

	~SymbolTable() { clear(); }

	void clear() { records->clear(); }

	void search(std::string identifier, SemanticRecord*, bool*);
	void insert(std::string identifier, SemanticRecord*);
	void print();



};