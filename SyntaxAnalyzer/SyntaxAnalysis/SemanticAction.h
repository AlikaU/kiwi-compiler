#pragma once
#include "GSymbol.h"

#define NUM_OF_SEMTYPES 4

class SemanticAction : public GSymbol {

public:
	~SemanticAction() {}
	enum SemanticActionTypes {
		makeProgTable = 0,
		makeClassTable = 1,
		makeFuncDefEntry = 2,
		makeVarEntry = 3,
		wrongType = -1
	};

	SemanticAction(SemanticActionTypes t, std::pair<int,int> p) {
		type = t;
		position = p;
	}

	bool isTerminal() const { return false; }
	SemanticActionTypes getType() const { return type; }
	static SemanticActionTypes stringToType(std::string s);
	bool isDollarSign() const { return false; }
	bool isEpsilon() const { return false; }
	std::pair<int, int> getPosition() { return position; }
	static std::string getSemanticTypeString(int idx) { return SemanticTypeStrings[idx]; }

private: 
	SemanticActionTypes type;
	std::pair<int, int> position;
	static std::string SemanticTypeStrings[NUM_OF_SEMTYPES];
};