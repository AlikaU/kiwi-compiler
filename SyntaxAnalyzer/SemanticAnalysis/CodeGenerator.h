#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "SemanticVariable.h"

#define EXECUTABLE_FOLDER "../Moon/moon/"

class CodeGenerator {

	std::ofstream exeFileStream;
	std::string executable_filepath;


public:

	CodeGenerator(std::string filename) {
		
		// erase log files from previous run		
		executable_filepath = EXECUTABLE_FOLDER + filename + ".m";
		remove(executable_filepath.c_str());		
	}

	CodeGenerator() {
		exeFileStream.close();
	}

	void genVariableDecl(SemanticVariable* varRecord);

	bool genExpr();

	bool genAssignment();

	bool genLoop();

	bool genConditional();

	bool genFunctionCall();

private:

};