#ifndef SIMPLE_ERROR_STRUCT
#define SIMPLE_ERROR_STRUCT

#include <string>

struct Error {
	std::string msg = "";
	explicit operator bool() const { return this->msg != ""; }
};

#endif