#include "SemanticAction.h"

std::string SemanticAction::SemanticTypeStrings[NUM_OF_SEMTYPES] = {
	"createProgTable",
	"startCollecting",
	"stopCollecting",
	"createSemanticClassAndTable",
	"calculateClassSize",
	"scopeIn",
	"scopeOut",
	"createSemanticVariable",
	"createSemanticFunctionAndTable", 
	"processAssignment", 
	"processVariableUse",
	"processTerm",
	"processExpression", 
	"processIdNestListIdThenIndiceListOrAParams",
	"processNum"
};

SemanticAction::SemanticActionTypes SemanticAction::stringExclMarkToType(std::string s) {
	s = s.substr(1, s.size() - 1);
	for (int i = 0; i < NUM_OF_SEMTYPES; ++i) {
		if (s == SemanticAction::SemanticTypeStrings[i]) {
			return static_cast<SemanticActionTypes>(i);
		}
	}
	return wrongType;
}

SemanticAction::SemanticActionTypes SemanticAction::stringToType(std::string s) {
	for (int i = 0; i < NUM_OF_SEMTYPES; ++i) {
		if (s == SemanticAction::SemanticTypeStrings[i]) {
			return static_cast<SemanticActionTypes>(i);
		}
	}
	return wrongType;
}

GSymbol * SemanticAction::clone() const {
	return new SemanticAction(*this);
}

GSymbol::GSymbolTypes SemanticAction::getSymbolType() {
	return GSymbol::semanticAction;
}