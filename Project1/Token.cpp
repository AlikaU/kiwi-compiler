#include "Token.h"
#include <string>
#include "Logger.h"


Token::Token(TokenTypes t, std::string v, std::pair<int, int> p) {
	type = t;
	value = v;
	position = p;

	Logger::getLogger()->log(Logger::LOG_TYPE::TOKEN, this->toString());
}

std::string Token::toString() {
	return TokenTypeStrings[static_cast<int>(type)] + ": \"" + value + "\", line " + 
		std::to_string(position.first) + ", column " + std::to_string(position.second);
}


const std::string Token::TokenTypeStrings[12] = { "INT", "FLOAT", "OPERATOR", "PUNCTUATION", "IDENTIFIER", "RESERVED_WORD"};