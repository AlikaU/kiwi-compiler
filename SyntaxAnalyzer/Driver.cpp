#include <iostream>
#include "ParseTable.h"

int main(int argc, char** argv)
{

	std::cout << "Hello Kiwi!"; 

	ParseTable pTable("../GrammarFiles/table.txt", "../GrammarFiles/rules.txt");
	std::getchar();
}