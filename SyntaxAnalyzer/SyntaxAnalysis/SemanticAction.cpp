#include "SemanticAction.h"

std::string SemanticAction::SemanticTypeStrings[NUM_OF_SEMTYPES] = {
	"makeProgTable",
	"makeClassTable",
	"makeFuncDefEntry",
	"makeVarEntry"
};

SemanticAction::SemanticActionTypes SemanticAction::stringToType(std::string s) {
	for (int i = 0; i < NUM_OF_SEMTYPES; ++i) {
		if (s == SemanticAction::SemanticTypeStrings[i]) {
			return static_cast<SemanticActionTypes>(i);
		}
	}
	return wrongType;
}