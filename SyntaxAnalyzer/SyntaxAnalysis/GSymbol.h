#pragma once
#include "..\..\Project1\Token.h"

class GSymbol {	

public: 
	enum GSymbolTypes {
		terminal = 0,
		nonTerminal = 1,
		semanticAction = 2
	};
	virtual ~GSymbol() {}                 
	virtual GSymbol * clone() const = 0;
	virtual GSymbolTypes getSymbolType() = 0;
	virtual bool isDollarSign() const = 0;
	virtual bool isEpsilon() const = 0;
};