#include "Scanner.h"
#include <iostream>     
#include <fstream>
#include <string>
#include "Logger.h"
#include "Token.h"



Token* Scanner::getNextToken() {
	state = StateTable::States::START;
	Token* token = NULL;
	char c;
	do {
		if (inputStream.get(c)) {
			updatePositionInFile(c);

			lastState = state;			
			state = StateTable::getState(state, c);

			std::string debugString = "current state is: " +
				StateTable::StateStrings[lastState] + " and we just read a ";
			debugString += c;
			debugString += " (ascii " + std::to_string((int)c) + ") ";
			Logger::getLogger()->log(Logger::LOG_TYPE::DEBUG, debugString);

			// If we're in state OUT_OF_DFA, we got out of the DFA
			// so we either completed a token or found an error
			if (state == StateTable::States::OUT_OF_DFA) {

				if (isCommentEnterOrExit(lastState)) {
					handleComment(lastState);
				}

				// Only create a token if we are not inside a comment
				// It's possible that we just read a "*/", 
				// so make sure to not create a token if it's the case
				if (!insideLineComment && nestedCommentLevel <= 0 && lastState != StateTable::States::CLOSE_COMMENT) {
					if (isValidToken(lastState, c)) {
						token = createToken(lastState, buffer);
						if (!token) {
							errorCount++;
							Logger::getLogger()->log(Logger::LOG_TYPE::ERROR,
								"Could not create token. This should not have happened.");
						}
					}					
				}
				if (lastState != StateTable::States::START) {
					backtrack();
				}
				buffer.clear();	
				state = StateTable::States::START;
			}
			else if (token == NULL && c != ' ' && StateTable::isCharLegal(c)) {
				buffer += c;
			}		

			// on newline, unset the comment flag if it was set
			if ((int)c == 10 && insideLineComment) {
				insideLineComment = false;
				Logger::getLogger()->log(Logger::LOG_TYPE::DEBUG, "\nExiting line comment\n");
			}
		}
		else {	
			Logger::getLogger()->log(Logger::LOG_TYPE::DEBUG, "End of file reached. Attempting to create a token with last characters read");
			
			if (nestedCommentLevel > 0) {
				errorCount++;
				Logger::getLogger()->log(Logger::LOG_TYPE::ERROR,
					"Comment unclosed at end of file.");
			}
			if (state == StateTable::States::START || insideLineComment || 
				nestedCommentLevel > 0 || lastState == StateTable::States::CLOSE_COMMENT) {
				Logger::getLogger()->log(Logger::LOG_TYPE::DEBUG, "No new tokens found");
				return new Token(Token::TokenTypes::DOLLAR_SIGN, "$", tokenPosition("$"));
			}
			else if (isValidToken(state, c)) {
				token = createToken(state, buffer);
				Logger::getLogger()->log(Logger::LOG_TYPE::DEBUG, "Last token found.");
				if (!token) {
					errorCount++;
					Logger::getLogger()->log(Logger::LOG_TYPE::ERROR,
						"Could not create token. This should not have happened.");
				}
			}
			else {
				Logger::getLogger()->log(Logger::LOG_TYPE::DEBUG, "No new tokens found");
				return new Token(Token::TokenTypes::DOLLAR_SIGN, "$", tokenPosition("$"));
			}
		}
	} while (token == NULL);

	return token;
}

void Scanner::handleComment(StateTable::States s) {
	if (s == StateTable::States::OPEN_COMMENT) {
		if (!insideLineComment) {
			nestedCommentLevel++;
			Logger::getLogger()->log(Logger::LOG_TYPE::DEBUG, "Entering block comment level " + std::to_string(nestedCommentLevel) + "\n");
		}
	}
	else if (s == StateTable::States::CLOSE_COMMENT) {		
		if (!insideLineComment) {

			// if we aren't in a block comment, this is an error
			if (nestedCommentLevel <= 0) {
				errorCount++;
				Logger::getLogger()->log(Logger::LOG_TYPE::ERROR, "Attempted to close comment without having opened one at line " +
					std::to_string(lastPosition.first) + ", column " + std::to_string(lastPosition.second -1));
				return;
			}

			Logger::getLogger()->log(Logger::LOG_TYPE::DEBUG, "Exiting block comment level " + std::to_string(nestedCommentLevel) + "\n");
			nestedCommentLevel--;
		}
	}
	else if (s == StateTable::States::LINE_COMMENT) {
		if (nestedCommentLevel <= 0) {
			Logger::getLogger()->log(Logger::LOG_TYPE::DEBUG, "Entering line comment\n");
			insideLineComment = true;
		}		
	}
}



bool Scanner::isCommentEnterOrExit(StateTable::States s) {
	return (s == StateTable::States::LINE_COMMENT || s == StateTable::States::OPEN_COMMENT || s == StateTable::States::CLOSE_COMMENT);
}

bool Scanner::isValidToken(StateTable::States s, char c) {

	// If non-final state, we have an error!
	if (!StateTable::isFinalState(s)) {
		std::string tokenString = buffer;

		// if in state START, the char isn't yet in the buffer. We still need to display it for error reporting
		if (s == StateTable::States::START) {
			tokenString = c;
		}

		std::pair<int, int> malformedTokenPosition = tokenPosition(tokenString);

		std::string errorDescription;
		std::string tokenMessage = "";
		if (s == StateTable::SAW_A_ZERO) {
			errorDescription = "Trailing zero(es) in float ";
			tokenMessage = "Token not created";
		} else if (s == StateTable::SOON_TO_BE_FLOAT) {
			errorDescription = "Malformed float ";
			tokenMessage = "Token not created";
		} else if (s == StateTable::START) {
			if (c == '_') {
				errorDescription = "Underscore not inside an identifier: ";
			}
			else {
				errorDescription = "Invalid character ";
			}
			malformedTokenPosition.second += 1;
		}
		
		errorCount++;
		Logger::getLogger()->log(Logger::LOG_TYPE::ERROR, errorDescription + "\"" + tokenString + "\" at line " +
			std::to_string(malformedTokenPosition.first) + ", column " + std::to_string(malformedTokenPosition.second) +
			". " + tokenMessage);

		return false;
	}	
	return true;
}

Token* Scanner::createToken(StateTable::States s, std::string value) {
	Token::TokenTypes type;

	if (s == StateTable::States::WORD) {
		if (isReservedWord(value)) {
			type = Token::TokenTypes::RESERVED_WORD;
		}
		else type = Token::TokenTypes::IDENTIFIER;
	}
	else {
		try{
			type = StateTable::FinalStateToTokenType.at(s);
		}
		catch(std::out_of_range){
			errorCount++;
			Logger::getLogger()->log(Logger::LOG_TYPE::ERROR, 
				"Could not convert DFA state to token type! This should not have happened.");
			return NULL;
		}
	}	

	return new Token(type, value, tokenPosition(value));
}

std::pair<int, int> Scanner::tokenPosition(std::string s) {
	std::pair<int, int> position;
	position.first = lastPosition.first;
	position.second = lastPosition.second - s.size() + 1;
	return position;
}

bool Scanner::isReservedWord(std::string s) {
	for (int i = 0; i < NUM_OF_RES_WORDS; i++) {
		if (s.compare(reservedWords[i]) == 0) {
			return true;
		}
	}
	return false;
}

void Scanner::updatePositionInFile(char c) {
	lastPosition = currentPosition;

	// if newline, increment line, else increment column
	if ((int)c == 10) {
		currentPosition.first++;
		currentPosition.second = 0;
	}
	else {
		currentPosition.second++;
	}
}

void Scanner::backtrack() {
	currentPosition = lastPosition;
	inputStream.unget();
}

