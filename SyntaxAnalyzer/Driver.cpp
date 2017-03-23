#include <iostream>
#include "SyntaxAnalysis\ParseTable.h"
#include "..\Project1\Scanner.h"
#include "SyntaxAnalysis\Parser.h"
#include "..\Utils\Logger.h"


// will significantly slow everything down
bool printDerivation = false;
bool printDerivationToConsole = false;
bool skipLongFiles = true;
bool success = true;

void testCorrectInput(char* path, ParseTable* pTable) {
	std::cout << "\n\nNow parsing valid file: " << path;
	if (printDerivation) {
		Logger::getLogger()->log(Logger::DERIV, "\n\nDerivation for valid file: " + std::string(path));
	}
	Scanner scanner(path);
	Parser parser(&scanner, pTable, printDerivation, printDerivationToConsole);
	if (parser.parse()) {
		std::cout << "\nSUCCESS";
	}
	else {
		std::cout << "\nFAILED";
		success = false;
	}
}


void testWrongInput(char* path, ParseTable* pTable) {
	std::cout << "\n\nNow parsing file with errors: " << path;
	if (printDerivation) {
		Logger::getLogger()->log(Logger::DERIV, "\n\nDerivation for file with errors: " + std::string(path));
	}
	Logger::getLogger()->log(Logger::ERROR, "Errors for file with errors: " + std::string(path));
	Scanner scanner(path);
	Parser parser(&scanner, pTable, printDerivation, printDerivationToConsole);
	if (!parser.parse()) {
		std::cout << "\nSUCCESS (parsing failed, as expected)";
	}
	else {
		std::cout << "\nFAILED (pasing succeeded when it should've failed";
		success = false;
	}
}

int main(int argc, char** argv)
{
	ParseTable pTable;

	std::cout << "\n\nIn case you need to see the derivation (from PREVIOUS ASSIGNMENT (2)), printing derivation to file works, but takes a very long time. You therefore have the option to print it to console instead.";
	std::cout << "\nPrint derivation to file (1), to console (2) or neither (3)?";
	int answer;
	do {		
		std::cin >> answer;
		if (std::cin.fail() || ! (answer >= 1 && answer <= 3) ) {
			std::cin.clear();
			std::cin.ignore(256, '\n');
			std::cout << "\nPlease enter a number between 1 and 3";
			std::cout << "\nPrint derivation to file (1), to console (2) or neither (3)?";
		}
	} while (!(answer >= 1 && answer <= 3));
	std::cin.clear();

	switch (answer) {
	case 1:
		printDerivation = true;
		printDerivationToConsole = false;
		break;
	case 2: 
		printDerivation = false;
		printDerivationToConsole = true;
		break;
	case 3:
		printDerivation = false;
		printDerivationToConsole = false;
		break;
	}	

	//testCorrectInput("../TestFiles/Syntax/good6.txt", &pTable);
	testCorrectInput("../TestFiles/Semantic/full_valid_program.txt", &pTable);
		

	if (success) {
		std::cout << "\n\nSUCCESS: All test cases passed succesfully!";
	}
	else {
		std::cout << "\n\nFAILED: At least one test case has failed.";
	}
	std::cout << "\nYou can find all log files in the Output folder.";
	
	std::cin >> answer;
}
