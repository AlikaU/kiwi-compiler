#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\Project1\Scanner.h"

#include "..\Project1\Scanner.cpp"
#include "..\Project1\Token.h"
#include "..\Project1\Token.cpp"
#include "..\Project1\StateTable.h"
#include "..\Project1\StateTable.cpp"
#include "..\Project1\Logger.h"
#include "..\Project1\Logger.cpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
//using namespace System;

namespace UnitTest_KiwiCompiler
{		
	TEST_CLASS(UnitTest1)
	{

		int token_ints = 0, token_floats = 0, token_operators = 0,
			token_punctuation = 0, token_identifiers = 0, token_reservedwords = 0;

		void initTest() {
			token_ints = token_floats = token_operators = token_punctuation = token_identifiers = token_reservedwords = 0;
		}

		void updateTokenCounts(Token* t) {
			if (t) {
				switch (t->getType()) {
				case Token::TokenTypes::INT:
					token_ints++;
					break;
				case Token::TokenTypes::FLOAT:
					token_floats++;
					break;
				case Token::TokenTypes::OPERATOR:
					token_operators++;
					break;
				case Token::TokenTypes::PUNCTUATION:
					token_punctuation++;
					break;
				case Token::TokenTypes::IDENTIFIER:
					token_identifiers++;
					break;
				case Token::TokenTypes::RESERVED_WORD:
					token_reservedwords++;
					break;
				}
			}			
		}

	public:
		
		TEST_METHOD(TestCorrectNumbers)
		{
			initTest();
			Scanner myScanner("../TestFiles/Lexical/numbers_correct.txt");
			while (myScanner.hasNext()) {
				updateTokenCounts(myScanner.getNextToken());
			}
			Assert::AreEqual(55, token_ints);
			Assert::AreEqual(26, token_floats);
			Assert::AreEqual(13, token_operators);
			Assert::AreEqual(13, token_punctuation);
			Assert::AreEqual(6, token_identifiers);
			Assert::AreEqual(0, token_reservedwords);
			Assert::AreEqual(0, myScanner.getErrorCount());
		}

		TEST_METHOD(TestErrorNumbers)
		{
			initTest();
			Scanner myScanner("../TestFiles/Lexical/numbers_errors.txt");			
			while (myScanner.hasNext()) {											
				updateTokenCounts(myScanner.getNextToken());							
			}		
			Assert::AreEqual(18, token_ints);
			Assert::AreEqual(2, token_floats);
			Assert::AreEqual(2, token_operators);
			Assert::AreEqual(2, token_punctuation);
			Assert::AreEqual(2, token_identifiers);
			Assert::AreEqual(0, token_reservedwords);
			Assert::AreEqual(30, myScanner.getErrorCount());
		}

		TEST_METHOD(TestCorrectIdsAndReservedWords)
		{
			initTest();
			Scanner myScanner("../TestFiles/Lexical/words_correct.txt");
			while (myScanner.hasNext()) {
				updateTokenCounts(myScanner.getNextToken());
			}
			Assert::AreEqual(9, token_ints);
			Assert::AreEqual(6, token_floats);
			Assert::AreEqual(21, token_operators);
			Assert::AreEqual(9, token_punctuation);
			Assert::AreEqual(83, token_identifiers);
			Assert::AreEqual(29, token_reservedwords);
			Assert::AreEqual(0, myScanner.getErrorCount());
		}

		TEST_METHOD(TestErrorIdsAndReservedWords)
		{
			initTest();
			Scanner myScanner("../TestFiles/Lexical/words_errors.txt");
			while (myScanner.hasNext()) {
				updateTokenCounts(myScanner.getNextToken());
			}
			Assert::AreEqual(0, token_ints);
			Assert::AreEqual(0, token_floats);
			Assert::AreEqual(0, token_operators);
			Assert::AreEqual(0, token_punctuation);
			Assert::AreEqual(7, token_identifiers);
			Assert::AreEqual(8, token_reservedwords);
			Assert::AreEqual(16, myScanner.getErrorCount());
		}

		TEST_METHOD(TestCorrectComments)
		{
			initTest();
			Scanner myScanner("../TestFiles/Lexical/comments_correct.txt");
			while (myScanner.hasNext()) {
				updateTokenCounts(myScanner.getNextToken());
			}
			Assert::AreEqual(0, token_ints);
			Assert::AreEqual(0, token_floats);
			Assert::AreEqual(0, token_operators);
			Assert::AreEqual(0, token_punctuation);
			Assert::AreEqual(5, token_identifiers);
			Assert::AreEqual(0, token_reservedwords);
			Assert::AreEqual(0, myScanner.getErrorCount());
		}

		TEST_METHOD(TestErrorComments)
		{
			initTest();
			Scanner myScanner("../TestFiles/Lexical/comments_errors.txt");
			while (myScanner.hasNext()) {
				updateTokenCounts(myScanner.getNextToken());
			}
			Assert::AreEqual(0, token_ints);
			Assert::AreEqual(0, token_floats);
			Assert::AreEqual(0, token_operators);
			Assert::AreEqual(0, token_punctuation);
			Assert::AreEqual(0, token_identifiers);
			Assert::AreEqual(0, token_reservedwords);
			Assert::AreEqual(3, myScanner.getErrorCount());
		}

		TEST_METHOD(TestOperatorsAndPunctuation)
		{
			initTest();
			Scanner myScanner("../TestFiles/Lexical/operators_punctuation.txt");
			while (myScanner.hasNext()) {
				updateTokenCounts(myScanner.getNextToken());
			}
			Assert::AreEqual(0, token_ints);
			Assert::AreEqual(0, token_floats);
			Assert::AreEqual(30, token_operators);
			Assert::AreEqual(14, token_punctuation);
			Assert::AreEqual(0, token_identifiers);
			Assert::AreEqual(0, token_reservedwords);
			Assert::AreEqual(16, myScanner.getErrorCount());
		}

	};
}