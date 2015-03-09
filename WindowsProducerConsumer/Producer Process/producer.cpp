#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
// producer-consumer using message passing synchronization - main module
// adapted from https://msdn.microsoft.com/en-us/library/windows/desktop/ms682499(v=vs.85).aspx
//
#include <windows.h> 
#include <iostream>
#include <sstream>
#include <map>

using namespace std;

#include "create_consumer_process.h"
#include "producer_pipe_io.h"
#include "format_error_message.h"

int main(int argc, char *argv[], char **env)
{
	if (argc < 2) {
		cerr << "?? must specify at least one argument - name of file to pass to child process" << endl;
		exit(1);
	}

	cout << endl << "Start of parent execution" << endl;
	int programStatus{ 0 };           // denotes success
	HANDLE hchild_stdout_read{ NULL }, hchild_stdout_write{ NULL };
	HANDLE hchild_stdin_read{ NULL }, hchild_stdin_write{ NULL };
	HANDLE ParentSendToChild{ NULL }, ChildGetsFromParent{ NULL };
	HANDLE ChildSendToParent{ NULL }, ParentGetsFromChild{ NULL };
	HANDLE hInputFile{ NULL };
	try {

		// Get a handle to an existing input file for use by the parent.
		// For details, see  https://msdn.microsoft.com/en-us/library/windows/desktop/aa363858%28v=vs.85%29.aspx
		// This example assumes a plain text file and uses string output to verify data flow. 
		//
		hInputFile = CreateFile(
			argv[1],                     // file name
			GENERIC_READ,                // desired access:  read and/or write
			0,                           // sharing mode - 0 means request exclusive access
			NULL,                        // security attributes - typically concerned with child processes inheriting handles
			OPEN_EXISTING,               // action to take based on file's existence or nonexistence
			FILE_ATTRIBUTE_READONLY,     // a far more specific access specification than parameter 2
			NULL);                       // a "template file" for characterizing file creation, if requested

		if (hInputFile == INVALID_HANDLE_VALUE) throw pair<string, string>("main: CreateFile", formatErrorMessage());

		// define security attributes parameters for child-process-inherited handles
		//   1st parameter must be as shown.
		//   2nd parameter - child process inherits parent process security attributes
		//   3rd parameter - child process inherits parent process's handles   
		// 
		SECURITY_ATTRIBUTES securityAttributes{ sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

		// Create a one-way anonymous pipe for passing data from child to parent
		// Prevent the child process from reading from this pipe.
		//
		if (!CreatePipe(&hchild_stdout_read, &hchild_stdout_write, &securityAttributes, 0))  throw pair<string, string>("main: CreatePipe (STDOUT)", formatErrorMessage());
		if (!SetHandleInformation(hchild_stdout_read, HANDLE_FLAG_INHERIT, 0))        throw pair<string, string>("main: SetHandleInformation (STDOUT)", formatErrorMessage());

		// Create a one-way anonymous pipe for passing data from parent to child.
		// Prevent the child process from writing to this pipe.
		//
		if (!CreatePipe(&hchild_stdin_read, &hchild_stdin_write, &securityAttributes, 0))    throw pair<string, string>("main: CreatePipe (STDIN)", formatErrorMessage());
		if (!SetHandleInformation(hchild_stdin_write, HANDLE_FLAG_INHERIT, 0))        throw pair<string, string>("main: SetHandleInformation (STDIN)", formatErrorMessage());

		// Create a pipe for passing ready or sleeping messages from child to parent
		//
		if (!CreatePipe(&ChildGetsFromParent, &ParentSendToChild, &securityAttributes, 0))
			throw pair<string, string>("main: CreatePipe (MSG STDOUT)", formatErrorMessage());
		if (!SetHandleInformation(ParentSendToChild, HANDLE_FLAG_INHERIT, 0))
			throw pair<string, string>("main: SetHandleInformation (MSG STDOUT)", formatErrorMessage());

		// Create a pipe for passing ready or sleeping messages from parent to child
		//
		if (!CreatePipe(&ParentGetsFromChild, &ChildSendToParent, &securityAttributes, 0))
			throw pair<string, string>("main: CreatePipe (MSG STDIN)", formatErrorMessage());
		if (!SetHandleInformation(ParentGetsFromChild, HANDLE_FLAG_INHERIT, 0))
			throw pair<string, string>("main: SetHandleInformation (MSG STDIN)", formatErrorMessage());

		// parse the HANDLEs for relaying messages to DWORDS 
		//
		DWORD cSendToParent{ reinterpret_cast<DWORD>(ChildSendToParent) };
		DWORD cGetFromParent{ reinterpret_cast<DWORD>(ChildGetsFromParent) };

		// start the child
		// pass the pipes for relaying messages as DWORDS to this method
		//
		CreateChildConsumerProcess(hchild_stdin_read, hchild_stdout_write, cSendToParent, cGetFromParent);

		// Write to the pipe that is the standard input for a child process. 
		// Data is written to the pipe's buffers, so it is not necessary to wait
		// until the child process is running before writing data.
		//
		WriteToPipe(hchild_stdin_write, ParentGetsFromChild, ParentSendToChild, hInputFile);
		cout << endl << "->Contents of " << argv[1] << " written to child STDIN pipe" << endl;

		//get the contents of the file from the consumer
		cout << endl << "->Contents of child process STDOUT:\n" << endl;
		ReadFromPipe(hchild_stdout_read, ParentGetsFromChild, ParentSendToChild);
		cout << endl << "\n->End of parent execution.\n\n" << endl;
	}
	catch (pair<string, string>& syndrome) {
		cerr << syndrome.first << ":" << syndrome.second << endl;
		programStatus = 1;           // denotes failure
	}

	CloseHandle(hInputFile);
	CloseHandle(hchild_stdin_read);
	CloseHandle(hchild_stdin_write);
	CloseHandle(hchild_stdout_read);
	CloseHandle(hchild_stdout_write);
	CloseHandle(ParentSendToChild);
	CloseHandle(ParentGetsFromChild);
	CloseHandle(ChildSendToParent);
	CloseHandle(ChildGetsFromParent);

	return programStatus;
}


