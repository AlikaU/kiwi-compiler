
Where to find input files: Project1\TestFiles
Where to find a copy of resulting outputs: Project1\ResultFiles


Compilation / execution instructions: 

1. Open the KiwiCompiler.sln file in Visual Studio 2015
2. You have two testing options:

2.1. Running the driver (you will see console output 
and log files will be generated)
	a. Open Project1\Project1\Driver.cpp
	b. On line 10, change the path to the input file of your choice
	c. Run the driver ("Play" button)
	d. You will find the log files generated by that run at 
		\Project1\Project1. They are called: 
		logError.txt, logTokens.txt and logDebug.txt
		(logDebug.txt wasn't required in assignment specifications)

2.2. Running the Unit tests
	a. In Visual Studio, open the Test Explorer
		(Test/Windows/Test Explorer)
	b. Click on "Run All"
	c. All tests should pass
	d. If you wish to see the tests, you can find them at 
		Project1\UnitTest_KiwiCompiler\test_scanner.cpp