// producer-consumer using message passing synchronization - main module
// adapted from https://msdn.microsoft.com/en-us/library/windows/desktop/ms682499(v=vs.85).aspx
//
#include <iostream>
#include <sstream>
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "producer_pipe_io.h"
#include "format_error_message.h"
#include "error_codes.h"

using namespace std;

int main(int argc, char *argv[], char **env)
{
	if (argc < 2) {
		cerr << "Error: must specify at least one argument - name of file to pass to child process" << endl;
		exit(1);
	}

	cout << "\n" << "Start of parent execution" << endl;
	int programStatus{ 0 };           // denotes success
	int stdout_read_write[2]; //parent reads from child, child writes to parent
	int stdin_read_write[2]; //child reads from parent, parent writes to child
	int c_gets_p_send[2]; //parent writes to child, child reads from parent
	int p_gets_c_send[2]; //child writes to parent, parent reads from child
	int hInputFile;
	pid_t consumerpid;
	try {
		// Get a handle to an existing input file for use by the parent.
		// This example assumes a plain text file and uses string output to verify data flow. 
		//
		hInputFile = open(argv[1], O_RDONLY);

		if (hInputFile == -1) 
		{
			cerr << "Error 40: " << format_error_message(openErrorCodes,errno) << endl;
			exit(2);
		}

		// Create a one-way anonymous pipe for passing data from child to parent
		// Prevent the child process from reading from this pipe.
		//
		if( pipe(stdout_read_write) == -1)
		{
			cerr << "Error 49: " << format_error_message(pipeErrorCodes, errno) << endl;
			exit(3);
		}

		// Create a one-way anonymous pipe for passing data from parent to child.
		// Prevent the child process from writing to this pipe.
		//
		if( pipe(stdin_read_write) == -1)
		{
			cerr << "Error 58: " << format_error_message(pipeErrorCodes, errno) << endl;
			exit(4);
		}

		// Create a pipe for passing ready or sleeping messages from child to parent
		//
		if( pipe(c_gets_p_send) == -1)
		{
			cerr << "Error 66: " << format_error_message(pipeErrorCodes, errno) << endl;
			exit(5);
		}

		// Create a pipe for passing ready or sleeping messages from parent to child
		//
		if( pipe(p_gets_c_send) == -1)
		{
			cerr << "Error 74: " << format_error_message(pipeErrorCodes, errno) << endl;
			exit(6);
		}
		
		//fork and create consumer process
		consumerpid = fork();
		if(consumerpid < 0)
		{
			cerr << "Error 82: " << format_error_message(forkErrorCodes, errno) << endl;
			exit(7);
		}
		//if fork was successful...
		if (consumerpid == 0) {
			//set childs standard input to the child reads from parent pipe
			//close the write end of the pipe so the child cant write to it
			if(dup2(stdin_read_write[0], STDIN_FILENO) < 0)
			{
				cerr << "Error line 90: " << format_error_message(dup2ErrorCodes, errno) << endl;
				exit(8);
			}
			//close the write side of this pipe
			close(stdin_read_write[1]);
			
			//set childs standard output to the child writes to parent pipe
			//close the read end of this pipe so the child cant read it
			if(dup2(stdout_read_write[1], STDOUT_FILENO) < 0)
			{ 
				cerr << "Error line 100: " << format_error_message(dup2ErrorCodes, errno) << endl;
				exit(9);
			}
			close(stdout_read_write[0]);
			
			//convert the file descriptors to strings for passing to the consumer
			stringstream ss;
			ss << p_gets_c_send[1];
			
			stringstream ss2;
			ss2 << c_gets_p_send[0];
			
			//execute consumer (child) process
			// *** *** manage child process *** ***
			execle("/home/bowersna/Assignment4/consumer", "consumer", ss.str().c_str(), ss2.str().c_str(), NULL, env);    
			cerr << "exec failed: " << format_error_message(execErrorCodes, errno) << endl;
			exit(10);    
		} 
			
		// Write to the pipe that is the standard input for a child process. 
		// Data is written to the pipe's buffers, so it is not necessary to wait
		// until the child process is running before writing data.
		//
		WriteToPipe(stdin_read_write[1], c_gets_p_send[1], p_gets_c_send[0], hInputFile);
		cout << endl << "->Contents of " << argv[1] << " written to child STDIN pipe" << endl;

		//get the contents of the file from the consumer
		cout << endl << "->Contents of child process STDOUT:\n" << endl;
		ReadFromPipe(stdout_read_write[0], p_gets_c_send[0], c_gets_p_send[1]);
		cout << endl << "\n->End of parent execution.\n\n" << endl;
	}
	catch (const char* syndrome) {
		cerr << syndrome << endl;;
		programStatus = 1;           // denotes failure
	}
	
	close(hInputFile);
	close(stdout_read_write[0]);
	close(stdout_read_write[1]);
	close(stdin_read_write[0]);
	close(stdin_read_write[1]);
	close(c_gets_p_send[0]);
	close(c_gets_p_send[1]);
	close(p_gets_c_send[0]);
	close(p_gets_c_send[1]);

	return programStatus;
}


