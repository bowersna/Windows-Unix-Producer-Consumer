// functions for reading set number of bytes from, writing set number of bytes to a handle 
// with non-overlapped I/O.
// includes error message generation and exception throwing when expected number of bytes not processes
//
#include "file_io.h"
#include "format_error_message.h"

#include <sstream>
#include <map>

using namespace std;

void writeToFile(HANDLE hFile, LPCVOID bufferOut, DWORD bytes_to_write, char * const callerName) {
	DWORD bytes_written;
	stringstream error_message_builder;
	if (!WriteFile(hFile, bufferOut, bytes_to_write, &bytes_written, NULL)) {
		error_message_builder << callerName << " - write failure";
		throw pair<string, string>(error_message_builder.str(), formatErrorMessage());
	}
	if (bytes_written != bytes_to_write) {
		error_message_builder << callerName << " - incomplete write: " << bytes_written << " of " << bytes_to_write << " bytes written";
		throw pair<string, string>(error_message_builder.str(), formatErrorMessage());
	}
}

void readFromFile(HANDLE hFile, LPVOID bufferIn, DWORD bytes_to_read, char * const callerName) {
	DWORD bytes_read;
	stringstream error_message_builder;
	if (!ReadFile(hFile, bufferIn, bytes_to_read, &bytes_read, NULL)) {
		error_message_builder << callerName << " - read failure";
		throw pair<string, string>(error_message_builder.str(), formatErrorMessage());
	}
	if (bytes_read != bytes_to_read) {
		error_message_builder << callerName << " - incomplete read: " << bytes_read << " of " << bytes_to_read << " bytes read";
		throw pair<string, string>(error_message_builder.str(), formatErrorMessage());
	}
}

