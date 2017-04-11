#include <iostream>
#include "SyntaxAnalysis\ParseTable.h"
#include "..\Project1\Scanner.h"
#include "SyntaxAnalysis\Parser.h"
#include "..\Utils\Logger.h"


// will significantly slow everything down
bool printDerivation = false;
bool printDerivationToConsole = false;
//bool skipLongFiles = true;
bool success = true;
int answer;
std::string answerStr;
bool askQs = true; // ask questions in console
bool askFile = true;
bool runAll = true;
const int goodProgramsIndex = 8;
std::string fileNames[] = { "CODEgen", "full_valid_program", "SEMmultipleid", "SEMtypecheckAsgExpSuccess", 
"SYNcomplicatedExpr", "SYNfreefunction", "SYNgood1", "SYNgood5", "SYNvarDecl"
"SEMtypecheckAsgExpError", "SEMtypemismatch", "SEMundefClass", "SEMundefFunction", "SEMundefVar", 
"SYNbad1", "SYNbad2", "SYNbad3", "SYNbad4", "SYNbad5"};
std::string inputFolder = "../TestFiles/DemoTestCases/";

std::string askForFilename();
void askQuestions();
void testCorrectInput(std::string folder, std::string filename, ParseTable* pTable);
void testWrongInput(std::string folder, std::string filename, ParseTable* pTable);

int main(int argc, char** argv)
{

	if (askQs) {
		askQuestions();
	}
	if (askFile) {
		askForFilename();
	}
	ParseTable pTable;
	if (!runAll) {
		testCorrectInput(inputFolder, answerStr, &pTable);
	}
	else {
		int i;
		for (i = 0; i < goodProgramsIndex; ++i) {
			testCorrectInput(inputFolder, fileNames[i], &pTable);
		}
		for (i; i < sizeof(fileNames); ++i) {
			testWrongInput(inputFolder, fileNames[i], &pTable);
		}
	}

	

	if (success) {
		std::cout << "\n\nSUCCESS: All test cases passed succesfully!";
	}
	else {
		std::cout << "\n\nFAILED: At least one test case has failed.";
	}
	std::cout << "\nYou can find all log files in the Output folder. Please see the symbol table in Output/logTable.txt and the semantic error output in Output/logSemanticError.txt";

	std::cin >> answer;
}

std::string askForFilename() {
	std::cout << "\nRun all input files (1) or enter filename manually (2)?";
	do {
		std::cin >> answer;
		if (std::cin.fail() || !(answer >= 1 && answer <= 2)) {
			std::cin.clear();
			std::cin.ignore(256, '\n');
			std::cout << "\nPlease enter 1 or 2";
			std::cout << "\nRun all input files (1) or enter filename manually (2)?";
		}
	} while (!(answer >= 1 && answer <= 2));
	std::cin.clear();
	if (answer == 2) {
		runAll = false;
		std::cout << "\nPlease type the name of input file";
		std::cin >> answerStr;
		while (true) {
			for (std::string fileName : fileNames) {
				if (answerStr == fileName)
					return answerStr;
			}
			// wrong answer
			std::cout << "\nThere is no such file. Try again";
			std::cin.clear();
			std::cin.ignore(256, '\n');
			std::cin >> answerStr;
		}
	}	
	return "";
}

void askQuestions() {
	std::cout << "\n\nIf you wish to see the derivation, printing derivation to file works, but takes a very long time. You therefore have the option to print it to console instead.";
	std::cout << "\nPrint derivation to file (1), to console (2) or neither (3)?";

	do {
		std::cin >> answer;
		if (std::cin.fail() || !(answer >= 1 && answer <= 3)) {
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
}


void testCorrectInput(std::string folder, std::string filename, ParseTable* pTable) {
	std::cout << "\n\nNow parsing valid file: " << folder << filename;

	Logger::getLogger()->log(Logger::TABLE, "\n\nSymbol Table for valid file: " + folder + filename + "\n");

	if (printDerivation) {
		Logger::getLogger()->log(Logger::DERIV, "\n\nDerivation for valid file: " + folder + filename);
	}
	
	Parser parser(pTable, printDerivation, printDerivationToConsole, folder, filename);
	if (parser.parse()) {
		std::cout << "\nSUCCESS";
	}
	else {
		std::cout << "\nFAILED";
		success = false;
	}
}


void testWrongInput(std::string folder, std::string filename, ParseTable* pTable) {
	std::cout << "\n\nNow parsing file with errors: " << folder << filename;
	Logger::getLogger()->log(Logger::TABLE, "\n\nSymbol Table for file with errors: " + folder + filename + "\n");
	if (printDerivation) {
		Logger::getLogger()->log(Logger::DERIV, "\n\nDerivation for file with errors: " + folder + filename);
	}
	Logger::getLogger()->log(Logger::ERROR, "Errors for file with errors: " + folder + filename);	
	Parser parser(pTable, printDerivation, printDerivationToConsole, folder, filename);
	if (!parser.parse()) {
		std::cout << "\nSUCCESS (parsing failed, as expected)";
	}
	else {
		std::cout << "\nFAILED (pasing succeeded when it should've failed";
		success = false;
	}
}




