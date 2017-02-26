#include <iostream>
#include "ParseTable.h"
#include "..\Project1\Scanner.h"
#include "Parser.h"




void testCorrectInput(char* path, ParseTable* pTable) {

	Scanner scanner(path);
	Parser parser(&scanner, pTable);
	if (parser.parse()) {
		std::cout << "\nParsing success for file " << path;
	}
	else {
		std::cout << "\nParsing failed for file " << path;
	}
}

int main(int argc, char** argv)
{
	ParseTable pTable;
	//testCorrectInput("../TestFiles/Syntax/full_valid_program.txt", &pTable);
	//testCorrectInput("../TestFiles/Syntax/good1.txt", &pTable);
	testCorrectInput("../TestFiles/Syntax/good2.txt", &pTable);
	testCorrectInput("../TestFiles/Syntax/good3.txt", &pTable);
	testCorrectInput("../TestFiles/Syntax/good4.txt", &pTable);
	std::getchar();
}
