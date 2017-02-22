#include <iostream>
#include "ParseTable.h"
#include "../Project1/Scanner.h"
#include "Parser.h"

int main(int argc, char** argv)
{

	std::cout << "Hello Kiwi!"; 

	ParseTable pTable("../GrammarFiles/table.txt", "../GrammarFiles/rules.txt");
	Scanner scanner("../TestFiles/numbers_errors.txt");
	Parser parser(&scanner, &pTable);


	std::getchar();
}