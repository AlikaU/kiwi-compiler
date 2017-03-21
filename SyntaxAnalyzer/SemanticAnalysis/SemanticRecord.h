#pragma once
#include <list>
#include "..\SyntaxAnalysis\GSymbol.h"
class SemanticRecord {


public:

	enum SemanticRecordType { INT = 0, FLOAT = 1, FUNCTION = 2, CLASS_T = 3 };
	std::string typeStrings[4] = { "INT", "FLOAT", "FUNCTION", "CLASS" };
	enum SemanticStructure { SIMPLE = 0, ARRAY = 1, CLASS_S = 2};
	std::string structureStrings[3] = { "SIMPLE", "ARRAY", "CLASS"};

	SemanticRecord(std::string, SemanticRecordType, SemanticStructure, int, int);
	~SemanticRecord() {}

	std::string getIdentifier() const { return identifier; }
	void print();
	virtual void printDetail() = 0;
	void setDeclared() {
		declared = true;
	}
	SemanticRecordType getSemanticType() const { return semanticType; }
	SemanticStructure getSemanticStructure() const { return semanticStructure; }
	int getArrayDimension() const { return arrayDimension; }
	int getAddress() const { return address; }
	

protected:
	SemanticRecordType semanticType;
	std::string identifier;
	bool declared;	
	SemanticStructure semanticStructure;
	int arrayDimension;
	int address;



};