#pragma once
#include <map>
#include "GSymbol.h"
#include "GNonTerminal.h"
#include "GTerminal.h"
#include <list>
#include <iostream>     
#include <fstream>
#define BUFLEN 255
#define SKIPLEN 3
#define TABLEROWS 57
#define TABLECOLUMNS 34

class ParseTable {

	// parsing table generated by http://hackingoff.com/compilers/predict-first-follow-set
	std::map<std::string, int> terminalsToIndices;
	int **table;

	// array of list containing the grammar rules
	// first element in the list is left side of the rule
	// remainder of the list is the the right side of the rule
	// rules generated by http://hackingoff.com/compilers/predict-first-follow-set
	// indices are off by 1, because array starts from 0, not from 1
	std::list<GSymbol*> *rules;

	bool init();
	std::ifstream tableFileStream;
	std::ifstream rulesFileStream;
	char buffer[BUFLEN];
	char terminalsToIndicesString[BUFLEN];
	bool initTerminalsToIndices();
	bool initTable();
	void  skipXcharsAndClearBuffer(int toSkip);
	int numOfTerminals;
	void initRules();
	void initLHSOfRule(int ruleNo);
	void initRHSOfRule(int ruleNo);
	int rulestringidx;
	int numRules;


public:
	ParseTable(std::string tablePath, std::string rulesPath) : tableFileStream(tablePath), 
		rulesFileStream(rulesPath), numOfTerminals(0) {
		
		if (!tableFileStream.is_open()) {
			std::cout << "Unable to open table input file";
		}
		if (!rulesFileStream.is_open()) {
			std::cout << "Unable to open rules input file";
		}
		if (!init()) {
			std::cout << "Could not initialize parsing table!";
			std::getchar();
			exit(1);
		}
	}
	int getRuleNo(GNonTerminal* nonterm, GTerminal* term);
	int getNumRules() { return numRules; }
	std::list<GSymbol*> getRule(int ruleNo) { return rules[ruleNo]; }
	~ParseTable (){} // TODO
};