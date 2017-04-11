#include "CodeGenerator.h"
#include <fstream>

//std::ofstream CodeGenerator::executableFile;

void CodeGenerator::genVariableDecl(SemanticVariable* varRecord) {
	// int or float
	if (varRecord->getSemanticType() == SemanticRecord::INT || varRecord->getSemanticType() == SemanticRecord::FLOAT) {
		std::ofstream exeFileStream(executable_filepath.c_str(), std::fstream::out | std::fstream::app);
		// not array
		if (varRecord->getArrayDimension().empty() == true) {

			if (exeFileStream.is_open()) {
				exeFileStream << "\n" << varRecord->getIdentifier() << "\t" << "dw 0";
			}
			else {
				std::cout << "\nCould not open executableFile for writing!";

			}
		}
		// array
		else {
			if (exeFileStream.is_open()) {
				int count = 1;
				for (int dim : varRecord->getArrayDimension()) {
					count = count*dim;
				}
				exeFileStream << "\n" << varRecord->getIdentifier() << "\t" << "res " << count;
			}
			else {
				std::cout << "\nCould not open executableFile for writing!";
			}
		}
	}
	// class
	else if (varRecord->getSemanticType() == SemanticRecord::CLASS_T) {

	}
	exeFileStream.flush();
}