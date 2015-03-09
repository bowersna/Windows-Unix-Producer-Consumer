// Format a system error message code as a string
// Parameters:
//      a map that maps error codes to strings, to use as a basis for error message generation 
//      an int that corresponds to a POSIX standard error code
// Return value:  a <string> object containing a message for the specified string, or "unknown error" if the code is missing from the map
//
// **> assumes multi-byte character set  (Properties > Configuration Properties > General > Character Set)
//
#ifndef __FORMAT_ERROR_MESSAGE_H__
#define __FORMAT_ERROR_MESSAGE_H__

#include <string>
#include <map>

#include <errno.h>

std::string format_error_message(std::map<int, std::string>, int = errno);
 
#endif