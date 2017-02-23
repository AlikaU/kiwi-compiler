#include <iostream>
#include "ParseTable.h"
#include "..\Project1\Scanner.h"
#include "Parser.h"

int main(int argc, char** argv)
{

	std::cout << "Hello Kiwi!"; 

	ParseTable pTable("../GrammarFiles/table.txt", "../GrammarFiles/rules.txt");
	Scanner scanner("../TestFiles/full_program.txt");
	Parser parser(&scanner, &pTable);
	if (parser.parse()) {
		std::cout << "parsing success!";
	}
	else {
		std::cout << "parsing failed";
	}

	std::getchar();
}