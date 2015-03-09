// Format a system error message code as a string
// Parameters:
//      none - default to GetLastError()  
//      DWORD - an error code to format
// Return value:  a <string> object containing a message for the specified string
//
// **> assumes multi-byte character set  (Properties > Configuration Properties > General > Character Set)
//
#ifndef __FORMAT_ERROR_MESSAGE_H__
#define __FORMAT_ERROR_MESSAGE_H__

#include <string>
#include <strsafe.h>

using namespace std;

string formatErrorMessage(bool = true);
string formatErrorMessage(DWORD, bool = true);

#endif