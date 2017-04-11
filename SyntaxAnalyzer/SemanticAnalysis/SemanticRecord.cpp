#include "SemanticRecord.h"
#include <string>

SemanticRecord::SemanticRecord(std::string i, SemanticRecordType st, 
	SemanticStructure ss, std::list<int>* arrD, int addr) {
	identifier = i;
	declared = false;
	semanticType = st;
	semanticStructure = ss;
	arrayDimension = arrD;
	address = addr;
}

void SemanticRecord::print() {
	std::string declaredStr = "false";
	if (declared) {
		declaredStr = "true";
	}
	Logger::getLogger()->log(Logger::TABLE, "\nRECORD: " + identifier + ", " + "declared: " + declaredStr + ", " + typeStrings[static_cast<int>(semanticType)] +
		", " + structureStrings[static_cast<int>(semanticStructure)]);
	//std::cout << "\nRECORD: " << identifier << ", " << declared  << ", " 
		//<< typeStrings[static_cast<int>(semanticType)]
		//<< ", " << structureStrings[static_cast<int>(semanticStructure)];
	if (arrayDimension != NULL) {
		if (arrayDimension->empty() == false) {
			Logger::getLogger()->log(Logger::TABLE, ", array dimension: ");
			for (int dim : *arrayDimension) {
				std::string str = "[" + dim;
				str = str + "]";
				Logger::getLogger()->log(Logger::TABLE, str);
			}
		}
	}

	printDetail();	
}

const std::string SemanticRecord::typeStrings[4] = { "INT", "FLOAT", "FUNCTION", "CLASS" };

const std::string SemanticRecord::structureStrings[3] = { "SIMPLE", "ARRAY", "CLASS" };