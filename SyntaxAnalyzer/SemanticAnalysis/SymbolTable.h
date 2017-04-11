#pragma once
#include "SemanticRecord.h"
#include <list>

class SymbolTable {

	SymbolTable* parent;
	std::list<SemanticRecord*>* records;
	std::string tableName;

public: 

	SymbolTable(std::string name) { tableName = name;  records = new std::list<SemanticRecord*>; }
	SymbolTable(SymbolTable* p, std::string name) { parent = p; records = new std::list<SemanticRecord*>; tableName = name; }

	~SymbolTable() { clearTable(); }
	std::string getTableName() const { return tableName; }
	void clearTable() { records->clear(); }
	SymbolTable* getParent() { return parent; }
	void search(std::string identifier, SemanticRecord**, bool&);
	bool insert(std::string identifier, SemanticRecord*);
	void print();



};