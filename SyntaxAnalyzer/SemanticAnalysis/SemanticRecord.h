#pragma once
#include <list>

class SemanticRecord {


public:
	enum SemanticRecordType { INT = 0, FLOAT = 1, FUNCTION = 2, CLASS = 3 };
	std::string typeStrings[4] = { "INT", "FLOAT", "FUNCTION", "CLASS" };
	enum SemanticStructure { SIMPLE = 0, ARRAY = 1, CLASS = 2};
	std::string structureStrings[3] = { "SIMPLE", "ARRAY", "CLASS" };
	std::string getIdentifier() const { return identifier; }
	virtual SemanticRecordType getType() = 0;
	void print();
	virtual void printDetail() = 0;

private:
	std::string identifier;
	bool declared;
	SemanticRecordType semanticType;
	SemanticStructure semanticStructure;
	int arrayDimension;
	int address;



};