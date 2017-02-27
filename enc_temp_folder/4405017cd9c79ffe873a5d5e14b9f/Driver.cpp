#include <iostream>
#include "ParseTable.h"
#include "..\Project1\Scanner.h"
#include "Parser.h"
#include "..\Utils\Logger.h"


// will significantly slow everything down
bool printDerivation = true;

void testCorrectInput(char* path, ParseTable* pTable) {
	std::cout << "\n\nNow parsing valid file: " << path;
	if (printDerivation) {
		Logger::getLogger()->log(Logger::DERIV, "\n\nDerivation for valid file: " + std::string(path));
	}
	Scanner scanner(path);
	Parser parser(&scanner, pTable, printDerivation);
	if (parser.parse()) {
		std::cout << "\nSUCCESS";
	}
	else {
		std::cout << "\nFAILED";
	}
}


void testWrongInput(char* path, ParseTable* pTable) {
	std::cout << "\n\nNow parsing file with errors: " << path;
	if (printDerivation) {
		Logger::getLogger()->log(Logger::DERIV, "\n\nDerivation for file with errors: " + std::string(path));
	}
	Logger::getLogger()->log(Logger::ERROR, "Errors for file with errors: " + std::string(path));
	Scanner scanner(path);
	Parser parser(&scanner, pTable, printDerivation);
	if (!parser.parse()) {
		std::cout << "\nSUCCESS (parsing failed, as expected)";
	}
	else {
		std::cout << "\nFAILED (pasing succeeded when it should've failed";
	}
}

int main(int argc, char** argv)
{

	ParseTable pTable;
	/**/
	testCorrectInput("../TestFiles/Syntax/full_valid_program.txt", &pTable);
	
	testCorrectInput("../TestFiles/Syntax/good1.txt", &pTable);
	testCorrectInput("../TestFiles/Syntax/good3.txt", &pTable);
	testCorrectInput("../TestFiles/Syntax/good4.txt", &pTable);
	

	testWrongInput("../TestFiles/Syntax/bad1.txt", &pTable);
	testWrongInput("../TestFiles/Syntax/bad2.txt", &pTable);
	testWrongInput("../TestFiles/Syntax/bad3.txt", &pTable);
	testWrongInput("../TestFiles/Syntax/bad4.txt", &pTable);

	
	std::cout << "\n\nDone parsing! Enjoy your parsed code.\nYou can find all log files in the Output folder.\nPress any key to exit.";
	std::getchar();
}
