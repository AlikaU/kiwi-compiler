#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\SyntaxAnalyzer\ParseTable.h"
#include "..\SyntaxAnalyzer\ParseTable.cpp"
#include "..\SyntaxAnalyzer\Parser.h"
#include "..\SyntaxAnalyzer\Parser.cpp"
#include "..\SyntaxAnalyzer\GNonTerminal.h"
#include "..\SyntaxAnalyzer\GNonTerminal.cpp"
#include "..\SyntaxAnalyzer\GTerminal.h"
#include "..\SyntaxAnalyzer\GTerminal.cpp"
#include "..\SyntaxAnalyzer\GSymbol.h"
#include "..\SyntaxAnalyzer\GSymbol.cpp"




using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UnitTest_KiwiCompiler
{
	TEST_CLASS(UnitTest1)
	{

	public:

		TEST_METHOD(TestFullValidProgram)
		{
			ParseTable pTable;
			Scanner scanner("../TestFiles/Syntax/full_valid_program.txt");
			Parser parser(&scanner, &pTable);
			Assert::IsTrue(parser.parse());			
		}

	

	};
}