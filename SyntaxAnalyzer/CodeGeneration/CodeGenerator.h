#pragma once

class CodeGenerator {

public:

	bool genVariableDecl();

	bool genExpr();

	bool genAssignment();

	bool genLoop();

	bool genConditional();

	bool genFunctionCall();
};