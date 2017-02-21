#include "GTerminal.h"

GTerminal::GTerminal(Token* t) {
	value = t->getValue();
	position = t->getPosition();
	switch (t->getType()) {
	case Token::TokenTypes::INT:
		type = GTerminal::TerminalTypes::INTNUM;
		break;
	case Token::TokenTypes::FLOAT:
		type = GTerminal::TerminalTypes::FLOATNUM;
		break;
	case Token::TokenTypes::OPERATOR:
		type = OperatorToTerminalType(t->getValue());
		break;
	case Token::TokenTypes::PUNCTUATION:
		type = PunctuationToTerminalType(t->getValue());
		break;
	case Token::TokenTypes::IDENTIFIER:
		type = GTerminal::TerminalTypes::ID;
		break;
	case Token::TokenTypes::RESERVED_WORD:
		type = ReservedWordToTerminalType(t->getValue());
		break;
	default:
		throw "Unknown token type " + type;
	}
}

GTerminal::TerminalTypes GTerminal::OperatorToTerminalType(std::string tempValue) {
	// switch statement doesn't work with strings :( 
	if (tempValue == "==")
		return GTerminal::TerminalTypes::EQ;
	else if (tempValue == "<>")
		return GTerminal::TerminalTypes::NE;
	else if (tempValue == "<")
		return GTerminal::TerminalTypes::LT;
	else if (tempValue == "<=")
		return GTerminal::TerminalTypes::LE;
	else if (tempValue == ">")
		return GTerminal::TerminalTypes::GT;
	else if (tempValue == ">=")
		return GTerminal::TerminalTypes::GE;
	else if (tempValue == ".")
		return GTerminal::TerminalTypes::DOT;
	else if (tempValue == "+")
		return GTerminal::TerminalTypes::PLUS;
	else if (tempValue == "-")
		return GTerminal::TerminalTypes::MINUS;
	else if (tempValue == "*")
		return GTerminal::TerminalTypes::MULT;
	else if (tempValue == "/")
		return GTerminal::TerminalTypes::DIVIDE;
	else if (tempValue == "=")
		return GTerminal::TerminalTypes::ASSIGN;
	else if (tempValue == "and")
		return GTerminal::TerminalTypes::AND;
	else if (tempValue == "not")
		return GTerminal::TerminalTypes::NOT;
	else if (tempValue == "or")
		return GTerminal::TerminalTypes::OR;
	else throw "Unknown operator value " + tempValue + "!";
}
GTerminal::TerminalTypes GTerminal::PunctuationToTerminalType(std::string tempValue) {
	if (tempValue == ";")
		return GTerminal::TerminalTypes::SEMI;
	else if (tempValue == ",")
		return GTerminal::TerminalTypes::COMMA;
	else if (tempValue == "(")
		return GTerminal::TerminalTypes::OPENPAR;
	else if (tempValue == ")")
		return GTerminal::TerminalTypes::CLOSEPAR;
	else if (tempValue == "{")
		return GTerminal::TerminalTypes::OPENCURLY;
	else if (tempValue == "}")
		return GTerminal::TerminalTypes::CLOSECURLY;
	else if (tempValue == "[")
		return GTerminal::TerminalTypes::OPENSQUARE;
	else if (tempValue == "]")
		return GTerminal::TerminalTypes::CLOSESQUARE;
	else throw "Unknown punctuation value " + tempValue + "!";
}
GTerminal::TerminalTypes GTerminal::ReservedWordToTerminalType(std::string tempValue) {
	if (tempValue == "if")
		return GTerminal::TerminalTypes::IF;
	else if (tempValue == "then")
		return GTerminal::TerminalTypes::THEN;
	else if (tempValue == "else")
		return GTerminal::TerminalTypes::ELSE;
	else if (tempValue == "for")
		return GTerminal::TerminalTypes::FOR;
	else if (tempValue == "class")
		return GTerminal::TerminalTypes::CLASS;
	else if (tempValue == "int")
		return GTerminal::TerminalTypes::INTWORD;
	else if (tempValue == "float")
		return GTerminal::TerminalTypes::FLOATWORD;
	else if (tempValue == "get")
		return GTerminal::TerminalTypes::GET;
	else if (tempValue == "put")
		return GTerminal::TerminalTypes::PUT;
	else if (tempValue == "return")
		return GTerminal::TerminalTypes::RETURN;
	else if (tempValue == "program")
		return GTerminal::TerminalTypes::PROGRAM;
	else throw "Unknown reserved word value " + tempValue + "!";
}