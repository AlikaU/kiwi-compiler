
Where to find input files: Project1/TestFiles/SemanticVerificationAndCodeGen
Where to find error reporting (semantic): Output/logSemanticError
Where to find symbol table: Output/logTable
Where to find generated Moon code files: Moon/moon
Where to find rest of documentation: Project1/Documentation/DocumentationAsg4.docx

Compilation / execution instructions: 

1. Open the KiwiCompiler.sln file in Visual Studio 2015

2. Run the driver of the SyntaxAnalyzer project 
	a. You will be given 2 options: run all test cases one after the other OR
		run each test case manually by typing its filename WITHOUT the .txt extesion
		(see TestFiles/SemanticVerificationAndCodeGen folder).
		Warning: output will be very messy because a lot of things will fail due to bugs!
	b. When grading error reporting, PLEASE choose the option to run tests one by one, 
		otherwise output will be very messy

Note: due to bugs and lack of time to fix them, in full_valid_program, the first pass of 
semantic verification succeeds, but the second one will give tons of errors. 

3. In case you want to see the derivation, the program will prompt you to either 
	- print derivation to a file (takes a while),
	- print derivation to console (a bit faster), 
	- or to not print it at all.




