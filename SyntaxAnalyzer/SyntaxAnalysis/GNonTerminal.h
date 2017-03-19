#pragma once
#include "GSymbol.h"
#define NUM_OF_NONTERMTYPES 60

class GNonTerminal : public GSymbol {


public: 

	GNonTerminal();
	
	enum NonTerminalTypes {
		prog = 0,
		makeProgTable = 1,
		classDeclList = 2,
		classDecl = 3,
		makeClassTable = 4,
		typeId = 5,
		varThenFuncList = 6,
		varThenFuncList1 = 7,
		varDeclList = 8,
		funcDefList = 9,
		progBody = 10,
		funcHead = 11,
		funcDef = 12,
		makeFuncDefEntry = 13,
		funcBody = 14,
		varThenStatList = 15,
		varThenStatList1 = 16,
		assignStatTail = 17,
		assignStatTail1 = 18,
		statementList = 19,
		varDecl = 20,
		makeVarEntry = 21,
		arraySizeList = 22,
		statement = 23,
		statementNotAssign = 24,
		assignStat = 25,
		statBlock = 26,
		expr = 27,
		exprTail = 28,
		arithExpr = 29,
		relExpr = 30,
		arithExprTail = 31,
		sign = 32,
		term = 33,
		termTail = 34,
		factor = 35,
		variable = 36,
		idNestListThenId = 37,
		idNestListThenId1 = 38,
		idNestListThenIdThenIndiceList = 39,
		idNestListThenIdThenIndiceList1 = 40,
		idNestListIdThenIndiceListOrAParams = 41,
		idNestListIdThenIndiceListOrAParams1 = 42,
		idNestListIdThenIndiceListOrAParams2 = 43,
		idNest = 44,
		idNestList = 45,
		indice = 46,
		indiceList = 47,
		arraySize = 48,
		type = 49,
		fParams = 50,
		fParamsTailList = 51,
		aParams = 52,
		aParamsTailList = 53,
		fParamsTail = 54,
		aParamsTail = 55,
		assignOp = 56,
		relOp = 57,
		addOp = 58,
		multOp = 59,		
		wrongType = -1
	};

	~GNonTerminal() {}
	GSymbol * clone() const;
	GNonTerminal(NonTerminalTypes t);
	bool isTerminal() const;
	NonTerminalTypes getType() const { return symbolType; }
	static NonTerminalTypes stringToType(std::string s);
	bool isDollarSign() const { return false; }
	bool isEpsilon() const { return false; }
	static std::string getNonTerminalTypeString(int idx) { return NonTerminalTypeStrings[idx]; }

private:

	NonTerminalTypes symbolType;
	static std::string NonTerminalTypeStrings[NUM_OF_NONTERMTYPES];
};