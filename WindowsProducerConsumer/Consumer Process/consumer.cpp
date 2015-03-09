// producer-consumer using message passing synchronization - consumer main module
// adapted from https://msdn.microsoft.com/en-us/library/windows/desktop/ms682499(v=vs.85).aspx
// **> assumes multi-byte character set  (Properties > Configuration Properties > General > Character Set)
//
#include "format_error_message.h"
#include "file_io.h"
#include <windows.h>
#include <sstream>
#include <iostream>

// **> assumes multi-byte character set  (Properties > Configuration Properties > General > Character Set)

using namespace std;

// Read output from the process's pipe for STDIN in one blast and write to the parent process's pipe for STDIN. 
// Read limited to maximum value that can be expressed in a DWORD
//
int main(int argc, char *argv[], char **env)
{
	//error message if the Producer process didn't supply 2 DWORD command line arguments
	if (argc < 3 || argc > 3)
	{
		stringstream errorMessageBuilder;
		errorMessageBuilder << "ERROR: (In Comsumer)\n\t -- must specify two HANDLE arguments for message passing -- " << endl;
		MessageBox(NULL, static_cast<LPCTSTR>(errorMessageBuilder.str().c_str()), TEXT("Error"), MB_OK);
		exit(1);
	}

	//reverse-parse the c string values to DWORDS then back to HANDLES
	DWORD ChildSendToParent{ static_cast<DWORD>(atoi(argv[1])) };
	DWORD ChildGetsFromParent{ static_cast<DWORD>(atoi(argv[2])) };

	HANDLE cSendToParent{ reinterpret_cast<HANDLE>(ChildSendToParent) };
	HANDLE cGetsFromParent{ reinterpret_cast<HANDLE>(ChildGetsFromParent) };

	int programStatus{ 0 };           // denotes success
	CHAR* fileContent{ NULL };
	CHAR* inMessage{ NULL };
	HANDLE hStdout{ NULL }, hStdin{ NULL };

	try {
		// get handles for communicating with parent process
		//
		hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hStdout == INVALID_HANDLE_VALUE)  throw pair<string, string>("main (GetStdHandle(STDOUT))", formatErrorMessage());

		hStdin = GetStdHandle(STD_INPUT_HANDLE);
		if (hStdin == INVALID_HANDLE_VALUE)   throw pair<string, string>("main (GetStdHandle(STDIN))", formatErrorMessage());

		//send "Ready to Recieve" message
		writeToFile(cSendToParent, "0", 1, "Consumer process main");

		//wait until consumer recieves "content is ready" message
		inMessage = new CHAR[1];
		readFromFile(cGetsFromParent, inMessage, 1, "Consumer process main");

		// determine size of input to process, allocating buffer for file read
		//
		// NOTE: *** *** this statement represents a potential race condition!! statement must execute after parent process does write *** ***
		DWORD inputSize{ GetFileSize(hStdin, NULL) };
		if (inputSize == INVALID_FILE_SIZE)  throw pair<string, string>("main (GetFileSize)", formatErrorMessage());
		fileContent = new CHAR[inputSize];

		// Read from standard input and stop on error or insufficient data, then
		// Write to standard output and stop on error.
		//

		//read from the input file using the appropriate stdin pipe
		readFromFile(hStdin, fileContent, inputSize, "Consumer process main");

		//accept a ready to recieve message from parent
		readFromFile(cGetsFromParent, inMessage, 1, "Consumer process main");
		
		//write the contents of the input file to the stdout pipe (going to producer)
		writeToFile(hStdout, fileContent, inputSize, "Consumer process main");
		
		//send the "content is ready" message to producer
		writeToFile(cSendToParent, "0", 1, "Consumer process main");
	}
	catch (pair<string, string>& syndrome) {
		// message box required for communication with user because process lacks window
		stringstream message_builder;
		message_builder << syndrome.first << ":" << syndrome.second << endl;
		MessageBox(NULL, static_cast<LPCTSTR>(message_builder.str().c_str()), TEXT("Error"), MB_OK);
		programStatus = 1;           // denotes failure
	}

	// clean up resources
	//
	CloseHandle(hStdout);
	CloseHandle(hStdin);
	delete[] fileContent;
	delete[] inMessage;

	return programStatus;
}
