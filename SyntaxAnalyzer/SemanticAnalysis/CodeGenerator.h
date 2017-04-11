#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "SemanticVariable.h"

#define EXECUTABLE_FOLDER "../Moon/moon/"

class CodeGenerator {

	std::ofstream exeFileStream;
	const char* executable_filepath;


public:

	CodeGenerator(std::string filename) {

		std::cout << "inside codegen constructor";

		// erase log files from previous run
		std::string str = EXECUTABLE_FOLDER + filename + ".m";
		str = "../" + str;
		executable_filepath = str.c_str();
		remove(executable_filepath);
		std::ofstream exeFileStream(executable_filepath, std::fstream::out | std::fstream::app);
		if (exeFileStream.is_open()) {
			exeFileStream << "\nWORKING";
		}
		else {
			std::cout << "\nCould not open executableFile for writing!";
		}
	}

	CodeGenerator() {
		exeFileStream.close();
	}

	bool genVariableDecl();

	bool genExpr();

	bool genAssignment();

	bool genLoop();

	bool genConditional();

	bool genFunctionCall();

private:

};