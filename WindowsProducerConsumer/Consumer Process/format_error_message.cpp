// Format a system error message code as a string
// Two variants:
//     Variant 1:
//         bool suppress_on_success - iff true, generate "" when GetLastError() == 0  (default) 
//         Return value:  a <string> object containing a message for GetLastError()    
//     Variant 2:
//         Parameters:
//             DWORD code - error code of the message to format
//             bool suppress_on_success - iff true, generate "" when code == 0  (default) 
//         Return value:  a <string> object containing a message for the specified string
//
// **> assumes multi-byte character set  (Properties > Configuration Properties > General > Character Set)
//
#include "format_error_message.h"
#include <sstream>
#include <string>
#include <windows.h>

string formatErrorMessage(bool suppress_on_success) {
	return formatErrorMessage(GetLastError(), suppress_on_success);
}

string formatErrorMessage(DWORD errorCode, bool suppress_on_success) {
	if (errorCode == 0 && suppress_on_success) return string{};

	stringstream error_message_builder{};
	void * msgBuf;

	// for FormatMessage, see https://msdn.microsoft.com/en-us/library/windows/desktop/ms679351%28v=vs.85%29.aspx
	//
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,                                         // location of the error message format string; no special formatting here
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&msgBuf,
		0,                                            // buffer size - not required with FORMAT_MESSAGE_ALLOCATE_BUFFER
		NULL                                          // array of values for format string arguments - not required here
		);

	error_message_builder << "error code " << errorCode << ": " << static_cast<char *>(msgBuf);
	LocalFree(msgBuf);                              // deallocate the buffer

	return string{ error_message_builder.str() };     // formatting complete
}

