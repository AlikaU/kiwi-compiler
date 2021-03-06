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
		RESERVED_WORD = 5,
		DOLLAR_SIGN = 6
	};
	static const std::string TokenTypeStrings[12];
	Token(TokenTypes t, std::string v, std::pair<int, int> p);
	std::string toString();
	TokenTypes getType() const { return type; }
	std::string getValue() const { return value; }
	std::pair<int, int> getPosition() const {return position; }

private:
	TokenTypes type;
	std::string value;
	std::pair<int, int> position;
	
};

