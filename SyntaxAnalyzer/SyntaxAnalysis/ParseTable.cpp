#include "ParseTable.h"

#include <stdexcept> 
#include "SemanticAction.h"

void  ParseTable::skipXcharsAndClearBuffer(int toSkip) {
	memset(buffer, 0, sizeof(buffer)); // clear buffer
	char c;
	for (int i = 0; i < toSkip; ++i) {
		if (!tableFileStream.get(c)) break;
	}
}




bool ParseTable::init() {
	initRules();
	return initTerminalsToIndices() && initTable();
}

int ParseTable::getRuleNo(GNonTerminal* nonterm, GTerminal* term) {

	if (term->getType() == GTerminal::CLOSESQUARE) {
		//std::cout << "here";
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
	rules[ruleNo].push_back(new GNonTerminal(GNonTerminal::stringToType(LHS)));

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
			rules[ruleNo].push_back(new GTerminal(GTerminal::TerminalTypes::EPSILON));
			//std::cout << "epsilon ";
		}
		// if it starts with a ', then we're dealing with a terminal
		else if (RHSelement[0] == '\'') {
			rules[ruleNo].push_back(new GTerminal(GTerminal::stringToType(RHSelement)));
			//std::cout << RHSelement << " ";
		}
		// if it starts with !, it is a semantic action
		else if (RHSelement[0] == '!') {
			rules[ruleNo].push_back(new SemanticAction(SemanticAction::stringExclMarkToType(RHSelement)));
		}
		// otherwise, it's a nonterminal
		else {
			rules[ruleNo].push_back(new GNonTerminal(GNonTerminal::stringToType(RHSelement)));
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