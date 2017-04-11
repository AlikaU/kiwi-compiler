#include "CodeGenerator.h"
#include <fstream>


//std::ofstream CodeGenerator::executableFile;

void CodeGenerator::genVariableDecl(SemanticVariable* varRecord) {
	if (varRecord->getSemanticType() == SemanticRecord::INT || varRecord->getSemanticType() == SemanticRecord::FLOAT) {
		std::ofstream exeFileStream(executable_filepath.c_str(), std::fstream::out | std::fstream::app);
		if (exeFileStream.is_open()) {
			exeFileStream << "\n" << varRecord->getIdentifier() << "\t" << "dw 0";
			exeFileStream.flush();
		}
		else {
			std::cout << "\nCould not open executableFile for writing!";
		}
		
	}
}