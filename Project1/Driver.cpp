#include "StateTable.h"
#include <iostream>
#include "..\Utils\Logger.h"
#include "Scanner.h"

int main(int argc, char** argv)
{
	//TestDFA();	

	Scanner scanner("../TestFiles/numbers_errors.txt");
	while (scanner.hasNext()) {
		Token* myToken = scanner.getNextToken();
		if (myToken) {
			std::cout << "\n" << myToken->toString();
		}		
	}

	std::cout << "\n\nDone scanning file. Enjoy your tokens!";

	
	std::cout.flush();
	std::getchar();
}

void TestDFA() {
	char inputChars[21] = { 'l','d','0',';',',','+','-','(',')','{','}','[',']','=','/','*','<','>','_','.',' ' };
	for (int j = 0; j < 29; j++) {
		std::cout << "=================================\n";
		std::cout << StateTable::StateStrings[j];
		std::cout << '\n';
		for (int i = 0; i < 21; i++) {
			StateTable::States state = StateTable::getState(static_cast<StateTable::States>(j), inputChars[i]);
			std::cout << StateTable::StateStrings[static_cast<int>(state)];
			std::cout << " ";
		}
		std::cout << "\n\n";
	}
}