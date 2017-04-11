#include "SemanticFunction.h"
#include <string>

SemanticFunction::SemanticFunction(std::string identifier,
	SemanticStructure sStruct, std::list<int>* arrDimension, int addr,
	std::list<SemanticVariable*>* prms, SymbolTable* lSymbols, SemanticType* rType)
	: SemanticRecord(identifier, SemanticRecord::FUNCTION, sStruct, arrDimension, addr) {
	params = prms;
	returnType = rType;
	int count = 0;
	for (SemanticVariable* var : *params) {
		++count;
	}
	numOfParams = count;
	localSymbolTable = lSymbols;
}

SemanticFunction::~SemanticFunction() {
	params->clear();
	delete returnType;
	delete params;
	delete localSymbolTable;
}

void SemanticFunction::printDetail() {
	Logger::getLogger()->log(Logger::TABLE, ", " + std::to_string(numOfParams) + " parameters: {");
	for (SemanticVariable* var : *params) {
		Logger::getLogger()->log(Logger::TABLE, typeStrings[static_cast<int>(var->getSemanticType())] + ", ");
	}
	Logger::getLogger()->log(Logger::TABLE, "}");

	if (localSymbolTable) {
		Logger::getLogger()->log(Logger::TABLE, "local symbol table: \n{");
		localSymbolTable->print();
		Logger::getLogger()->log(Logger::TABLE, "\n}\n");
	}
	else Logger::getLogger()->log(Logger::TABLE, "no local symbol table.");
}

bool SemanticFunction::hasParam(std::string identifier) {
	bool found = false;;
	for (SemanticVariable* var : *params) {
		if (var->getIdentifier() == identifier) {
			found = true;
			break;
		}
	}
	return found;
}

SemanticVariable* SemanticFunction::getParam(std::string identifier) {
	for (SemanticVariable* var : *params) {
		if (var->getIdentifier() == identifier) {
			return var;
		}
	}
	return NULL;
}
