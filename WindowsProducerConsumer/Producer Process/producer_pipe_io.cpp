#include "producer_pipe_io.h"
#include "file_io.h"
#include "format_error_message.h"

#include <iostream>
#include <sstream>
using namespace std;

// **> assumes multi-byte character set  (Properties > Configuration Properties > General > Character Set)

// Read a file's content in one blast and write its entire contents to the pipe for the child's STDIN.
// Write limited to maximum value that can be expressed in a DWORD
//
void WriteToPipe(HANDLE stdin_write, HANDLE ParentGetFromChild, HANDLE ParentSendToChild, HANDLE hInputFile)
{
	CHAR* fileContent{ NULL };
	CHAR* inMessage{ NULL };
	try {
		// determine size of input file, assuming file is not more than DWORD_MAX bytes
		// see https://msdn.microsoft.com/en-us/library/windows/desktop/aa364955%28v=vs.85%29.aspx
		//
		DWORD fileSize{ GetFileSize(hInputFile, NULL) };
		if (fileSize == INVALID_FILE_SIZE) throw pair<string, string>("WriteToPipe: GetFileSize", formatErrorMessage());

		// *** *** the writeToFile statement represents a potential race condition!! write must execute before child process does read *** ***
		// *** *** to observe the race, delay this next statement for a few seconds with a Sleep()--say, Sleep(6000)--or a breakpoint and pause before continuing *** ***
		// *** *** program should then stall at read file below *** ***
		//
		
		//recieve content from input file to producer (pipe.txt on my computer)
		fileContent = new CHAR[fileSize];
		readFromFile(hInputFile, fileContent, fileSize, "WriteToPipe");
		
		//read the "ready to recieve" message from the consumer
		inMessage = new CHAR[1];
		readFromFile(ParentGetFromChild, inMessage, 1, "WriteToPipe");
	
		//"ready to recieve" message was read from appropriate pipe
		//write file contents to stdin write handle
		writeToFile(stdin_write, fileContent, fileSize, "WriteToPipe");
		
		//send "content is ready" message
		writeToFile(ParentSendToChild, "0", 1, "WriteToPipe");

		delete[] fileContent;
		delete[] inMessage;
	}
	catch (pair<string, string>& syndrome) {
		delete[] fileContent;
		throw syndrome;
	}
}

// Read output from the child process's pipe for STDOUT in one blast and write to the parent process's pipe for STDOUT. 
// Read limited to maximum value that can be expressed in a DWORD
//
void ReadFromPipe(HANDLE stdout_read, HANDLE ParentGetsFromChild, HANDLE ParentSendToChild)
{
	CHAR* fileContent{ NULL };
	CHAR* inMessage{ NULL };
	try {
		// get handle for displaying results of read
		//
		HANDLE hStdout{ GetStdHandle(STD_OUTPUT_HANDLE) };
		if (hStdout == INVALID_HANDLE_VALUE) throw pair<string, string>("WriteToPipe: GetStdHandle(STDOUT)", formatErrorMessage());

		//send a "ready to recieve" message
		writeToFile(ParentSendToChild, "0", 1, "ReadFromPipe");

		//accept a "content is ready" message from consumer
		// this ensures we will get the correct file size
		inMessage = new CHAR[1];
		readFromFile(ParentGetsFromChild, inMessage, 1, "ReadFromPipe");

		// determine size of input to process, allocating buffer for file read
		//
		DWORD inputSize{ GetFileSize(stdout_read, NULL) };
		if (inputSize == INVALID_FILE_SIZE)  throw pair<string, string>("WriteToPipe: GetFileSize", formatErrorMessage());
		fileContent = new CHAR[inputSize];

		readFromFile(stdout_read, fileContent, inputSize, "ReadFromPipe");
		writeToFile(hStdout, fileContent, inputSize, "ReadFromPipe");

		delete[] fileContent;
		delete[] inMessage;
	}
	catch (pair<string, string>& syndrome) {
		delete[] fileContent;
		throw syndrome;
	}
}




