#pragma once
#include "GSymbol.h"

#define NUM_OF_SEMTYPES 18

class SemanticAction : public GSymbol {

public:
	~SemanticAction() {}
	enum SemanticActionTypes {
		createProgTable = 0,
		startCollecting = 1,
		stopCollecting = 2,
		createSemanticClassAndTable = 3,
		calculateClassSize = 4,
		scopeIn = 5,
		scopeOut = 6,
		createSemanticVariable = 7,
		createSemanticFunctionAndTable = 8,
		processAssignment = 9,
		processVariableUse = 10,
		processTerm = 11,
		processExpression = 12,		
		processIdNestListIdThenIndiceListOrAParams = 13,
		processIndiceList = 14,
		processArithExpr = 15,
		processRelExpr = 16,
		processNum = 17,		
		wrongType = -1
	};

	SemanticAction(SemanticActionTypes t, std::pair<int,int> p) {
		type = t;
		position = p;
	}

	SemanticAction(SemanticActionTypes t) {
		type = t;
		position.first = 0;
		position.second = 0;
	}

	SemanticAction(SemanticAction* action) {
		type = action->getType();
		position.first = action->getPosition().first;
		position.second = action->getPosition().second;
	}

	bool isTerminal() const { return false; }
	SemanticActionTypes getType() const { return type; }
	static SemanticActionTypes stringToType(std::string s);
	static SemanticActionTypes stringExclMarkToType(std::string s);
	bool isDollarSign() const { return false; }
	bool isEpsilon() const { return false; }
	std::pair<int, int> getPosition() { return position; }
	std::string getSemanticTypeString() { return SemanticTypeStrings[static_cast<int>(type)]; }
	GSymbol * clone() const;
	GSymbolTypes getSymbolType();

private: 
	SemanticActionTypes type;
	std::pair<int, int> position;
	static std::string SemanticTypeStrings[NUM_OF_SEMTYPES];
};