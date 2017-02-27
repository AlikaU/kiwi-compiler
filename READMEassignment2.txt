
Where to find input files: Project1/TestFiles/Syntax
Where to find the grammar, FIRST and FOLLOW sets: Project1/Documentation
Where to find rest of documentation: Project1/Documentation/Documentation.docx
Where to find a copy of the derivation output file: 

Compilation / execution instructions: 

1. Open the KiwiCompiler.sln file in Visual Studio 2015
2. Run the driver of the SyntaxAnalyzer project (you will see console output 
and log files will be generated)
	a. The driver runs all test cases
	b. The program will output the derivations for all files in
		Output/logDerivation.txt 
3. To toggle derivation output on / off, change the printDerivation
	boolean in the driver (Project1/SyntaxAnalyzer/Driver.cpp). 
	Turning it on will make it run for a very long time;
	I included a copy of the derivation output file, if you don't want to wait.

Notes:
 
- The derivation document will be very long
- Testing using driver, because unit tests crash for some reason. Will fix later.


