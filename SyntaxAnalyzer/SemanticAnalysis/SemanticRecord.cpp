#include "SemanticRecord.h"
#include <iostream>
#include <string>

void SemanticRecord::print() {
	std::string declaredStr = "false";
	if (declared) {
		declaredStr = "true";
	}
	std::cout << "RECORD: " << identifier << ", " << declared  << ", " 
		<< typeStrings[static_cast<int>(semanticType)]
		<< ", " << structureStrings[static_cast<int>(semanticStructure)];

	printDetail();

	if (arrayDimension > 1) {
		std::cout << ", array dimension: " << arrayDimension;
	}
}