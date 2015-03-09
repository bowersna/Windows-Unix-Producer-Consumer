#include "producer_pipe_io.h"
#include "file_io.h"
#include "format_error_message.h"
#include "error_codes.h"

#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

// **> assumes multi-byte character set  (Properties > Configuration Properties > General > Character Set)

// Read a file's content in one blast and write its entire contents to the pipe for the child's STDIN.
// Write limited to maximum value that can be expressed in a DWORD
//
void WriteToPipe(int stdout_write, int ParentSendToChild, int ParentGetFromChild, int hInputFile)
{
	const char *const myName{ "WriteToPipe" };
	char* fileContent{ NULL };
	int fileSize;
	struct stat st;

	try {
		//get file size from inputFile
		if(fstat(hInputFile, &st))
		{
			cerr << "Error (writeToPipe 31): " << format_error_message(fstatErrorCodes, errno) << endl;
			exit(11);
		}
		//if successful size is int st.st_size
		fileSize = st.st_size;
		
		//recieve content from input file to producer (pipe.txt on my computer)
		fileContent = new char[fileSize];
		readFromFile(hInputFile, fileContent, fileSize, myName);
		
		//read the "ready to recieve" message from the consumer
		char inMessage;
		readFromFile(ParentGetFromChild, &inMessage, 1, myName);
	
		//"ready to recieve" message was read from appropriate pipe
		//write file contents to stdout write handle
		writeToFile(stdout_write, fileContent, fileSize, myName);
		
		//send "content is ready" message
		writeToFile(ParentSendToChild, &fileSize, sizeof(fileSize), myName);
	
		delete[] fileContent;
	}
	catch (const char* syndrome) {
		delete[] fileContent;
		throw syndrome;
	}
}

// Read output from the child process's pipe for STDOUT in one blast and write to the parent process's pipe for STDOUT. 
// Read limited to maximum value that can be expressed in a DWORD
//
void ReadFromPipe(int stdin_read, int ParentGetsFromChild, int ParentSendToChild)
{
	char* fileContent{ NULL };
	int inputFileSize;
	int fileSize;
	try {
		//send a "ready to recieve" message
		writeToFile(ParentSendToChild, "0", 1, "ReadFromPipe");

		//accept a "content is ready" message from consumer
		// this ensures we will get the correct file size
		readFromFile(ParentGetsFromChild, &inputFileSize, sizeof(inputFileSize), "ReadFromPipe");
		
		//set the filesize from input message
		fileSize = inputFileSize;
		fileContent = new char[fileSize];
		
		//read from the consumer STDOUT and then write to the producer STDOUT
		readFromFile(stdin_read, fileContent, fileSize, "ReadFromPipe");
		writeToFile(STDOUT_FILENO, fileContent, fileSize, "ReadFromPipe");

		delete[] fileContent;
	}
	catch (const char* syndrome) {
		delete[] fileContent;
		throw syndrome;
	}
}




