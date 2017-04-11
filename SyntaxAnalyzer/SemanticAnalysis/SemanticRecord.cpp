#include "SemanticRecord.h"
#include <string>

SemanticRecord::SemanticRecord(std::string i, SemanticRecordType st, 
	SemanticStructure ss, std::list<int> arrD, int addr, std::pair<int, int> pos) {
	identifier = i;
	declared = false;
	semanticType = st;
	semanticStructure = ss;

	arrayDimension = std::list<int>(arrD);
	address = addr;
	position.first = pos.first;
	position.second = pos.second;
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
	
	if (arrayDimension.empty() == false) {
		Logger::getLogger()->log(Logger::TABLE, ", array dimension: ");
		for (int dim : arrayDimension) {
			Logger::getLogger()->log(Logger::TABLE, "[" + std::to_string(dim) + "]");
		}
	}
	

	printDetail();	
}

const std::string SemanticRecord::typeStrings[4] = { "INT", "FLOAT", "FUNCTION", "CLASS" };

const std::string SemanticRecord::structureStrings[3] = { "SIMPLE", "ARRAY", "CLASS" };