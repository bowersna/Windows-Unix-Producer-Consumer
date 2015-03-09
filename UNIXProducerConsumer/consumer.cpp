// producer-consumer using message passing synchronization - consumer main module
// **> assumes multi-byte character set  (Properties > Configuration Properties > General > Character Set)
//
#include "format_error_message.h"
#include "file_io.h"
#include <sstream>
#include <iostream>
#include <unistd.h>

using namespace std;
// **> assumes multi-byte character set  (Properties > Configuration Properties > General > Character Set)

// Read output from the process's pipe for STDIN in one blast and write to the parent process's pipe for STDIN. 
// Read limited to maximum value that can be expressed in a DWORD
//
int main(int argc, char *argv[], char **env)
{
	//error message if the Producer process didn't supply 2 DWORD command line arguments
	if (argc < 3 || argc > 3)
	{
		cerr << " Error: Invalid number of command line arguments. Must specify 2.\n" << endl;
		exit(1);
	}
	
	int programStatus{ 0 };           // denotes success
	char* fileContent{ NULL };
	int inputFileSize;
	char inMessage;
	// acting as HANDLES
	int cSendToParent, cGetsFromParent;
	
	//get the file descriptors for the pipes from the command line arguments
	cSendToParent = atoi(argv[1]);
	cGetsFromParent = atoi(argv[2]);
	
	try {
		//send "Ready to Recieve" message
		writeToFile(cSendToParent, "0", 1, "Consumer process main");
			
		//wait until consumer recieves "content is ready" message
		readFromFile(cGetsFromParent, &inputFileSize, sizeof(inputFileSize), "Consumer process main");
		
		fileContent = new char[inputFileSize];
	
		// Read from standard input and stop on error or insufficient data, then
		// Write to standard output and stop on error.
		//
		
		//read from the input file using the appropriate stdin pipe
		readFromFile(STDIN_FILENO, fileContent, inputFileSize, "Consumer process main");
		
		//accept a ready to recieve message from parent
		readFromFile(cGetsFromParent, &inMessage, 1, "Consumer process main");
		
		//write the contents of the input file to the stdout pipe (going to producer)
		writeToFile(STDOUT_FILENO, fileContent, inputFileSize, "Consumer process main");
		
		//send the "content is ready" message to producer
		//send the fileSize as the message
		writeToFile(cSendToParent, &inputFileSize, sizeof(inputFileSize), "Consumer process main");
	}
	catch (const char* syndrome) {
		// message box required for communication with user because process lacks window
		cerr << syndrome << endl;
		programStatus = 1;           // denotes failure
	}

	// clean up resources
	//
	close(cSendToParent);
	close(cGetsFromParent);
	delete[] fileContent;

	return programStatus;
}
