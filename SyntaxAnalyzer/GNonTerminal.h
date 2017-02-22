#pragma once
#include "GSymbol.h"
#define NUM_OF_NONTERMTYPES 56

class GNonTerminal : public GSymbol {


public: 

	GNonTerminal();
	
	enum NonTerminalTypes {
		prog = 0,
		classDeclList = 1,
		classDecl = 2,
		typeId = 3,
		varThenFuncList = 4,
		varThenFuncList1 = 5,
		varDeclList = 6,
		funcDefList = 7,
		progBody = 8,
		funcHead = 9,
		funcDef = 10,
		funcBody = 11,
		varThenStatList = 12,
		varThenStatList1 = 13,
		assignStatTail = 14,
		assignStatTail1 = 15,
		statementList = 16,
		varDecl = 17,
		arraySizeList = 18,
		statement = 19,
		statementNotAssign = 20,
		assignStat = 21,
		statBlock = 22,
		expr = 23,
		exprTail = 24,
		arithExpr = 25,
		relExpr = 26,
		arithExprTail = 27,
		sign = 28,
		term = 29,
		termTail = 30,
		factor = 31,
		variable = 32,
		idNestListThenId = 33,
		idNestListThenId1 = 34,
		idNestListThenIdThenIndiceList = 35,
		idNestListThenIdThenIndiceList1 = 36,
		idNestListIdThenIndiceListOrAParams = 37,
		idNestListIdThenIndiceListOrAParams1 = 38,
		idNestListIdThenIndiceListOrAParams2 = 39,
		idNest = 40,
		idNestList = 41,
		indice = 42,
		indiceList = 43,
		arraySize = 44,
		type = 45,
		fParams = 46,
		fParamsTailList = 47,
		aParams = 48,
		aParamsTailList = 49,
		fParamsTail = 50,
		aParamsTail = 51,
		assignOp = 52,
		relOp = 53,
		addOp = 54,
		multOp = 55,
		wrongType = -1
	};

	
	GNonTerminal(NonTerminalTypes t);
	bool isTerminal() const;
	NonTerminalTypes getType() const { return type; }
	static NonTerminalTypes stringToType(std::string s);

private:

	NonTerminalTypes symbolType;
	static std::string NonTerminalTypeStrings[NUM_OF_NONTERMTYPES];
};