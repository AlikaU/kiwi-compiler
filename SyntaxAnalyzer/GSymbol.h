#pragma once
#include "..\Project1\Token.h"

class GSymbol {	

public: 
	virtual bool isTerminal() const = 0;
	virtual bool isDollarSign() const = 0;
};