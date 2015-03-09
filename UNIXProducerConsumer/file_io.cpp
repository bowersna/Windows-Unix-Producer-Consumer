// functions for reading set number of bytes from, writing set number of bytes to a handle 
// with non-overlapped I/O.
// includes error message generation and exception throwing when expected number of bytes not processes
//
#include "file_io.h"
#include "format_error_message.h"
#include "error_codes.h"

#include <sstream>
#include <iostream>
#include <map>
#include <unistd.h>

using namespace std;

void writeToFile(int hFile, const void* bufferOut, int bytes_to_write, const char * const callerName) 
{
	int bytes_written;
	stringstream error_message_builder;
	
	bytes_written = write(hFile, bufferOut, bytes_to_write);
	if (bytes_written == -1) {
		error_message_builder << callerName << " - write failure";
		cerr << error_message_builder << " " << format_error_message(writeErrorCodes, errno) << endl;
	}
	if (bytes_written != bytes_to_write)
	{
		error_message_builder << callerName << " - incomplete write: " << bytes_written << " of " << bytes_to_write << " bytes written";
		cerr << error_message_builder << endl;
	}
}

void readFromFile(int hFile, void* bufferIn, int bytes_to_read, const char * const callerName) 
{
	int bytes_read;
	stringstream error_message_builder;
	
	bytes_read = read(hFile, bufferIn, bytes_to_read);
	if (bytes_read == -1) {
		error_message_builder << callerName << " - read failure";
		cerr << error_message_builder << " " << format_error_message(readErrorCodes, errno) << endl;
	}
	if (bytes_read != bytes_to_read) {
		error_message_builder << callerName << " - incomplete read: " << bytes_read << " of " << bytes_to_read << " bytes read";
		cerr << error_message_builder << endl;
	}
}

