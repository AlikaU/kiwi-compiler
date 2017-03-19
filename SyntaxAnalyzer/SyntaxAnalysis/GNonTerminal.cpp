#include "GNonTerminal.h"

GNonTerminal::GNonTerminal() {

}
GNonTerminal::GNonTerminal(NonTerminalTypes t) { nonTerminalType = t; }

std::string GNonTerminal::NonTerminalTypeStrings[NUM_OF_NONTERMTYPES] = {
	"prog",
	"classDeclList",
	"classDecl",
	"typeId",
	"varThenFuncList",
	"varThenFuncList1",
	"varDeclList",
	"funcDefList",
	"progBody",
	"funcHead",
	"funcDef",
	"funcBody",
	"varThenStatList",
	"varThenStatList1",
	"assignStatTail",
	"assignStatTail1",
	"statementList",
	"varDecl",
	"arraySizeList",
	"statement",
	"statementNotAssign",
	"assignStat",
	"statBlock",
	"expr",
	"exprTail",
	"arithExpr",
	"relExpr",
	"arithExprTail",
	"sign",
	"term",
	"termTail",
	"factor",
	"variable",
	"idNestListThenId",
	"idNestListThenId1",
	"idNestListThenIdThenIndiceList",
	"idNestListThenIdThenIndiceList1",
	"idNestListIdThenIndiceListOrAParams",
	"idNestListIdThenIndiceListOrAParams1",
	"idNestListIdThenIndiceListOrAParams2",
	"idNest",
	"idNestList",
	"indice",
	"indiceList",
	"arraySize",
	"type",
	"fParams",
	"fParamsTailList",
	"aParams",
	"aParamsTailList",
	"fParamsTail",
	"aParamsTail",
	"assignOp",
	"relOp",
	"addOp",
	"multOp"
};


bool GNonTerminal::isTerminal() const { return false; }

GNonTerminal::NonTerminalTypes GNonTerminal::stringToType(std::string s) {
	for (int i = 0; i < NUM_OF_NONTERMTYPES; ++i) {
		if (s == GNonTerminal::NonTerminalTypeStrings[i]) {
			return static_cast<NonTerminalTypes>(i);
		}
	}
	return wrongType;
}

GSymbol * GNonTerminal::clone() const {
	return new GNonTerminal(*this);
}

GSymbol::GSymbolTypes GNonTerminal::getSymbolType() {
	return GSymbol::nonTerminal;
}
