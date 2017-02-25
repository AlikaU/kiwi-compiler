#pragma once
#include "..\Project1\Token.h"

class GSymbol {	

public: 
	virtual ~GSymbol() {}                 
	virtual GSymbol * clone() const = 0;
	virtual bool isTerminal() const = 0;
	virtual bool isDollarSign() const = 0;
	virtual bool isEpsilon() const = 0;
};