#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\SyntaxAndSemanticAnalyzer\SyntaxAnalysis\ParseTable.h"
#include "..\SyntaxAndSemanticAnalyzer\SyntaxAnalysis\ParseTable.cpp"
#include "..\SyntaxAndSemanticAnalyzer\SyntaxAnalysis\Parser.h"
#include "..\SyntaxAndSemanticAnalyzer\SyntaxAnalysis\Parser.cpp"
#include "..\SyntaxAndSemanticAnalyzer\SyntaxAnalysis\GNonTerminal.h"
#include "..\SyntaxAndSemanticAnalyzer\SyntaxAnalysis\GNonTerminal.cpp"
#include "..\SyntaxAndSemanticAnalyzer\SyntaxAnalysis\GTerminal.h"
#include "..\SyntaxAndSemanticAnalyzer\SyntaxAnalysis\GTerminal.cpp"
#include "..\SyntaxAndSemanticAnalyzer\SyntaxAnalysis\GSymbol.h"
#include "..\SyntaxAndSemanticAnalyzer\SyntaxAnalysis\GSymbol.cpp"




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