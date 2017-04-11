#include "SemanticClass.h"
#include <string>

SemanticClass::SemanticClass(std::string identifier, std::list<int> arrDimension, int addr, SymbolTable* lSymbols, std::pair<int, int> position)
	: SemanticRecord(identifier, SemanticRecordType::CLASS_T, SemanticStructure::CLASS_S, arrDimension, addr, position) {

	localSymbolTable = lSymbols;
}

SemanticClass::~SemanticClass() {
	delete localSymbolTable;
}

void SemanticClass::printDetail() {

	if (localSymbolTable) {
		Logger::getLogger()->log(Logger::TABLE, ", local symbol table: \n{");
		localSymbolTable->print();
		Logger::getLogger()->log(Logger::TABLE, "\n}\n");
	}
	else Logger::getLogger()->log(Logger::TABLE, ", no local symbol table.");
}