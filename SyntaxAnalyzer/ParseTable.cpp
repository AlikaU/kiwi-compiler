#include "ParseTable.h"

#include <stdexcept> 




/*
const std::map < std::string, int > ParseTable::terminalsToIndices = {
	{ "EPSILON",0 },
	{ "class",1 },
	{ "id",2 },
	{ "{",3 },
	{ "}",4 },
	{ ";",5 },
	{ "(",6 },
	{ ")",7 },
	{ "program",8 },
	{ "int",9 },
	{ "float",10 },
	{ ".",11 },
	{ "if",12 },
	{ "then",13 },
	{ "else",14 },
	{ "+",15 },
	{ "-",16 },
	{ "numInt",17 },
	{ "numFloat",18 },
	{ "[",19 },
	{ "]",20 },
	{ ",",21 },
	{ "=",22 },
	{ "==",23 },
	{ "<>",24 },
	{ "<",25 },
	{ ">",26 },
	{ "<=",27 },
	{ ">=",28 },
	{ "or",29 },
	{ "*",30 },
	{ "/",31 },
	{ "and",32 },
	{ "$",33 }
};
*/

void  ParseTable::skipXcharsAndClearBuffer(int toSkip) {
	memset(buffer, 0, sizeof(buffer)); // clear buffer
	char c;
	for (int i = 0; i < toSkip; ++i) {
		if (!tableFileStream.get(c)) break;
	}
}




bool ParseTable::init() {
	/*
	table[57][34] = {
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,1,100,100,100,100,100,100,1,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,99 },
		{ 0,2,100,100,100,100,100,100,3,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,4,100,100,100,100,100,100,99,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,5,100,100,99,99,100,100,5,5,100,100,100,100,100,100,100,100,99,100,100,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,6,100,7,100,100,100,100,6,6,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,100,100,99,8,9,100,100,100,100,100,100,100,100,100,100,100,100,8,100,100,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,10,100,100,100,100,100,100,10,10,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,12,100,13,100,100,100,100,12,12,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,13 },
		{ 0,100,100,100,100,100,100,100,14,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,99 },
		{ 0,100,15,99,100,100,100,100,100,15,15,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,16,100,99,100,100,100,100,16,16,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,99 },
		{ 0,100,100,17,100,99,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,100,100,99,100,100,100,100,18,19,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,20,100,100,100,100,100,100,100,100,21,100,100,100,100,100,100,100,21,100,100,21,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,100,100,100,100,100,100,100,100,100,22,100,100,100,100,100,100,100,22,100,100,22,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,100,100,100,100,100,100,100,100,100,24,100,100,100,100,100,100,100,100,100,100,23,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,25,100,26,100,100,100,100,100,100,100,25,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,27,100,100,100,100,100,100,27,27,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,100,100,100,29,100,29,100,100,100,100,100,100,100,100,100,100,100,28,100,29,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,30,100,99,99,100,100,100,100,100,100,31,100,99,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,99,100,99,99,100,100,100,100,100,100,32,100,99,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,33,100,100,99,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,35,34,100,36,100,100,100,100,100,100,35,100,36,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,37,100,100,99,37,99,100,100,100,100,100,100,100,100,100,37,37,100,100,99,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,100,100,100,40,100,40,100,100,100,100,100,100,100,38,38,100,100,100,100,40,100,39,39,39,39,39,39,38,100,100,100,100 },
		{ 0,100,41,100,100,99,41,99,100,100,100,100,100,100,100,100,100,41,41,100,99,99,100,99,99,99,99,99,99,100,100,100,100,100 },
		{ 0,100,42,100,100,100,42,100,100,100,100,100,100,100,100,100,100,42,42,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,100,100,100,44,100,44,100,100,100,100,100,100,100,43,43,100,100,100,44,44,100,44,44,44,44,44,44,43,100,100,100,100 },
		{ 0,100,100,100,100,100,100,100,100,100,100,100,100,100,100,45,46,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,47,100,100,99,47,99,100,100,100,100,100,100,100,99,99,47,47,100,99,99,100,99,99,99,99,99,99,99,100,100,100,100 },
		{ 0,100,100,100,100,48,100,48,100,100,100,100,100,100,100,48,48,100,100,100,48,48,100,48,48,48,48,48,48,48,49,49,49,100 },
		{ 0,100,50,100,100,99,53,99,100,100,100,100,100,100,100,99,99,51,52,100,99,99,100,99,99,99,99,99,99,99,99,99,99,100 },
		{ 0,100,54,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,99,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,55,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,100,100,100,100,100,100,100,100,100,56,100,100,100,100,100,100,100,56,100,100,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,58,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,99,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,100,100,100,100,100,100,100,100,100,59,100,100,100,100,100,100,100,100,100,100,60,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,61,100,100,99,100,99,100,100,100,100,100,100,100,99,99,100,100,100,99,99,100,99,99,99,99,99,99,99,99,99,99,100 },
		{ 0,100,100,100,100,99,63,99,100,100,100,62,100,100,100,99,99,100,100,62,99,99,100,99,99,99,99,99,99,99,99,99,99,100 },
		{ 0,100,100,100,100,65,100,65,100,100,100,64,100,100,100,65,65,100,100,100,65,65,100,65,65,65,65,65,65,65,65,65,65,100 },
		{ 0,100,66,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,67,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,100,100,100,99,100,99,100,100,100,99,100,100,100,99,99,100,100,69,99,99,99,99,99,99,99,99,99,99,99,99,99,100 },
		{ 0,100,100,100,100,71,100,71,100,100,100,71,100,100,100,71,71,100,100,70,71,71,71,71,71,71,71,71,71,71,71,71,71,100 },
		{ 0,100,100,100,100,99,100,99,100,100,100,100,100,100,100,100,100,100,100,72,100,99,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,75,100,100,100,100,100,100,73,74,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,76,100,100,100,100,77,100,76,76,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,100,100,100,100,100,79,100,100,100,100,100,100,100,100,100,100,100,100,100,78,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,80,100,100,100,80,81,100,100,100,100,100,100,100,100,100,80,80,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,100,100,100,100,100,83,100,100,100,100,100,100,100,100,100,100,100,100,100,82,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,100,100,100,100,100,99,100,100,100,100,100,100,100,100,100,100,100,100,100,84,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,100,100,100,100,100,99,100,100,100,100,100,100,100,100,100,100,100,100,100,85,100,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,99,100,100,100,99,100,100,100,100,100,100,100,100,100,100,99,99,100,100,100,86,100,100,100,100,100,100,100,100,100,100,100 },
		{ 0,100,99,100,100,100,99,100,100,100,100,100,100,100,100,100,100,99,99,100,100,100,100,87,88,89,90,91,92,100,100,100,100,100 },
		{ 0,100,99,100,100,100,99,100,100,100,100,100,100,100,100,93,94,99,99,100,100,100,100,100,100,100,100,100,100,95,100,100,100,100 },
		{ 0,100,99,100,100,100,99,100,100,100,100,100,100,100,100,100,100,99,99,100,100,100,100,100,100,100,100,100,100,100,96,97,98,100 }

	};
	*/

	initRules();
	return initTerminalsToIndices() && initTable();
}

int ParseTable::getRuleNo(GNonTerminal* nonterm, GTerminal* term) {
	if (term->getValue() == "+") {
		std::cout << "here";
	}
	
	// +1, because the 0th row is filled with zeroes in our table,
	// so we shift everything by 1
	int nontermindice = static_cast<int>(nonterm->getType()) + 1;
	int termindice;
	try {
		termindice = terminalsToIndices.at(GTerminal::getTerminalTypeStringNoQuotes(static_cast<int>(term->getType())));
		return table[nontermindice][termindice];
	}
	catch(const std::out_of_range& oor){
		std::cerr << "Out of Range error: " << oor.what() << '\n';
		std::cerr << "Token that caused error: " << term->getValue();
		std::getchar();
		exit(1);
	}	
}

void ParseTable::initRules() {
	rules = new std::list<GSymbol*>[NUM_RULES + 1];
	numRules = 1;
	memset(buffer, 0, sizeof(buffer)); // clear buffer
	rulesFileStream.getline(buffer, BUFLEN);
	while (buffer) {

		rulestringidx = 0;
		initLHSOfRule(numRules);
		initRHSOfRule(numRules);

		numRules++;
		rulesFileStream.getline(buffer, BUFLEN);
		//std::cout << "\n\nline " << numRules << ": " << buffer << "\n";
		if (buffer[0] == '\0') {
			break;
		}
	}
	rulesFileStream.close();
	--numRules;

	/*

	std::cout << "\nHERE ARE THE RULES:";
	for (int i = 0; i < 99; i++) {
		std::list<GSymbol*> rule = rules[i];
		for (GSymbol* symbol : rule) {
			if (symbol->isTerminal()) {
				GTerminal* term = static_cast<GTerminal*>(symbol);
				std::cout << GTerminal::getTerminalTypeString(static_cast<int>(term->getType()));
			}
			else {
				GNonTerminal* nonterm = static_cast<GNonTerminal*>(symbol);
				std::cout << GNonTerminal::getNonTerminalTypeString(static_cast<int>(nonterm->getType()));
			}
			std::cout << ", ";
		}
		std::cout << "\n";
	}
	*/
}

void ParseTable::initLHSOfRule(int ruleNo) {
	// skip chars until you see a first letter, which is the LHS of the rule
	char c = buffer[rulestringidx];
	while ((int)c < 97 || (int)c > 122) {
		++rulestringidx;
		c = buffer[rulestringidx];
	}

	// get LHS (left hand side) of the rule
	std::string LHS = "";
	while (c != ' ') {
		LHS += c;
		c = buffer[++rulestringidx];
	}
	//std::cout << "\nLHS of rule: " << LHS;
	GNonTerminal* nonterm = new GNonTerminal(GNonTerminal::stringToType(LHS));
	rules[ruleNo].push_back(nonterm);

	// skip 3 chars, which are " -> "
	rulestringidx += 5;
}

void ParseTable::initRHSOfRule(int ruleNo) {
	//std::cout << "\nRHS of rule: ";

	// read nonterminals until you see a tab character
	char c = buffer[rulestringidx];
	while ((int)c != 9) {

		// read characters until you see a space or a tab
		std::string RHSelement = "";
		while ((int)c != 32 && (int)c != 9) {
			RHSelement += c;
			c = buffer[++rulestringidx];
		}
		

		// if we have a negative char, it's cause we see an epsilon character
		if (RHSelement[0] < 0) {
			GTerminal* term = new GTerminal(GTerminal::TerminalTypes::EPSILON);
			rules[ruleNo].push_back(term);
			//std::cout << "epsilon ";
		}
		// if it starts with a ', then we're dealing with a terminal
		else if (RHSelement[0] == '\'') {
			GTerminal* term = new GTerminal(GTerminal::stringToType(RHSelement));
			rules[ruleNo].push_back(term);
			//std::cout << RHSelement << " ";
		}
		// otherwise, it's a nonterminal
		else {
			GNonTerminal* nonterm = new GNonTerminal(GNonTerminal::stringToType(RHSelement));
			rules[ruleNo].push_back(nonterm);
			//std::cout << RHSelement << " ";
		}

		// skip the spaces between symbols
		if (c != 9) {
			c = buffer[++rulestringidx];
		}		
	}
	memset(buffer, 0, sizeof(buffer)); // clear buffer
	rulestringidx = 0;
}

bool ParseTable::initTable() {
	table = new int*[TABLEROWS];
	int row = 0;
	tableFileStream.get(buffer, BUFLEN, ']');
	while (buffer) {

		if (row > TABLEROWS) {
			std::cout << "Unexpected number of rows!";
			return false;
		}

		table[row] = new int[TABLECOLUMNS];

		int column = 0;

		int count = 0;
		std::string word = "";
		while (buffer[count] != '\0') {
			if (buffer[count] != ',') {
				word += buffer[count];
			}
			else {
				try {
					table[row][column] = stoi(word);
				}
				catch (const std::invalid_argument& ia) {
					std::cout << "\nInvalid argument while parsing table: " << ia.what() << '\n';
					return false;
				}
				++column;
				word.clear();
			}
			++count;
		}
		try {
			table[row][column] = stoi(word);
		}
		catch (const std::invalid_argument& ia) {
			std::cout << "\nInvalid argument while parsing table: " << ia.what() << '\n';
			return false;
		}

		skipXcharsAndClearBuffer(3);
		++row;
		tableFileStream.get(buffer, BUFLEN, ']');
		if (buffer[0] == '\0') {
			break;
		}
	}
	skipXcharsAndClearBuffer(0);
	std::cout << "\nInitialized parsing table.";
	tableFileStream.close();
	return true;
}

bool ParseTable::initTerminalsToIndices() {
	// read first row of table, it is our terminalsToIndices conversion chart
	tableFileStream.get(buffer, BUFLEN, ']');
	strncpy_s(terminalsToIndicesString, BUFLEN, buffer, BUFLEN);
	memset(buffer, 0, sizeof(buffer)); // clear buffer
	tableFileStream.get(buffer, BUFLEN, '$');
	strcat_s(terminalsToIndicesString, buffer);
	memset(buffer, 0, sizeof(buffer)); // clear buffer
	tableFileStream.get(buffer, BUFLEN, ']');
	strcat_s(terminalsToIndicesString, buffer);

	// init terminalsToIndices
	int numOfTerminals = 1;
	terminalsToIndices.insert(std::pair<std::string, int>("EPSILON", 0));
	std::string word;
	for (int i = 6; i < sizeof(terminalsToIndicesString); i++) {
		if (terminalsToIndicesString[i] == 0) {
			break;
		}
		if (terminalsToIndicesString[i] == '"') {
			terminalsToIndices.insert(std::pair<std::string, int>("$", numOfTerminals));
			if (numOfTerminals >(TABLECOLUMNS - 1)) {
				std::cout << "\nUnexpected number of terminals!";
				return false;
			}
			break;
		}
		if (terminalsToIndicesString[i] != '\'') {
			word += terminalsToIndicesString[i];
		}
		else {
			terminalsToIndices.insert(std::pair<std::string, int>(word, numOfTerminals));
			++numOfTerminals;
			i += 4; // skip ","'
			word.clear();
		}
	}



	// cleanup 
	skipXcharsAndClearBuffer(3);
	return true;
}