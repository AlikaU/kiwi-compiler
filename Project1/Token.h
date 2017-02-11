#pragma once
#include <string>

class Token {
public:
	enum TokenTypes : int {
		INT = 0,
		FLOAT = 1,
		OPERATOR = 2,
		PUNCTUATION = 3,
		IDENTIFIER = 4,
		RESERVED_WORD = 5
	};
	static const std::string TokenTypeStrings[12];
	Token(TokenTypes t, std::string v, std::pair<int, int> p);
	std::string toString();
	TokenTypes getType() const { return type; }

private:
	TokenTypes type;
	std::string value;
	std::pair<int, int> position;
	
};

