
Where to find input files: Project1/TestFiles/Syntax
Where to find the grammar, FIRST and FOLLOW sets: Project1/Documentation
Where to find rest of documentation: Project1/Documentation/DocumentationAsg2.docx

Compilation / execution instructions: 

1. Open the KiwiCompiler.sln file in Visual Studio 2015

2. Run the driver of the SyntaxAnalyzer project 
	a. The driver runs all test cases
	b. The program will (optionally) output the derivations for all files in
		Output/logDerivation.txt 
	c. The program will ouput all errors in Output/logError.txt

3. The program will prompt you to either 
	- print derivation to a file (takes a while, I have to fix this),
	- print derivation to console (a bit faster), 
	- or to not print it at all. 

For some reason the file output was very slow, 
	that's why I added an option to print it to console.
	
Notes:

- Testing from driver, because unit tests crash for some reason. Will fix later.
- The derivation document is very long.



