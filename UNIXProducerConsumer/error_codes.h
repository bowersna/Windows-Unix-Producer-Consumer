#ifndef __ERROR_CODES_H__
#define __ERROR_CODES_H__ 

#include <map>
#include <string>

// associative arrays that map error codes for fork, exec, and wait to descriptive strings
//
extern std::map<int, std::string> forkErrorCodes;
extern std::map<int, std::string> execErrorCodes;
extern std::map<int, std::string> pipeErrorCodes;
extern std::map<int, std::string> dup2ErrorCodes;
extern std::map<int, std::string> openErrorCodes;
extern std::map<int, std::string> readErrorCodes;
extern std::map<int, std::string> writeErrorCodes;
extern std::map<int, std::string> fstatErrorCodes;

#endif