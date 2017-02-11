#pragma once
#include "Token.h"
#include "StateTable.h"
#include <iostream>     
#include <fstream>
#include <string>
#include <utility> 

#define NUM_OF_RES_WORDS 14

class Scanner {
	std::ifstream inputStream;
	std::string reservedWords[NUM_OF_RES_WORDS] = {"and", "not", "or", "if", "then", "else", "for",
		"class", "int", "float", "get", "put", "return", "program"};
	StateTable::States state;
	StateTable::States lastState;
	std::string buffer;	
	std::pair<int,int> currentPosition;
	std::pair<int, int> lastPosition;
	int nestedCommentLevel;
	bool insideLineComment;
	int errorCount;

	bool isReservedWord(std::string s);
	std::pair<int, int> tokenPosition(std::string s);
	bool isValidToken(StateTable::States s, char c);
	bool Scanner::isCommentEnterOrExit(StateTable::States s);
	Token* createToken(StateTable::States s, std::string value);
	void handleComment(StateTable::States s);
	void updatePositionInFile(char c);
	void backtrack();
	
public:
	Scanner(char const* path) : inputStream(path), currentPosition(1, 0), 
		lastPosition(1,0), nestedCommentLevel(0), insideLineComment(false), 
		errorCount(0) {
		if (!inputStream.is_open()) {
			std::cout << "Unable to open input file";
		}
	}
	int getErrorCount() const { return errorCount; }
	~Scanner() { if(inputStream.is_open()) inputStream.close(); }
	Token* getNextToken();
	bool hasNext() { return (bool)inputStream; }
};