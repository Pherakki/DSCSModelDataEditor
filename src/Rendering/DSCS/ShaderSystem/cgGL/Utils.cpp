#include <exception>
#include <iostream>
#include <sstream>

#include "Utils.hpp"

// checkForCgError
const void checkForCgError(const CGcontext& context, const std::string& message)
{
	CGerror error;
	std::stringstream ss;
	const char* stringout = cgGetLastErrorString(&error);

	if (error != CG_NO_ERROR)
	{
		ss << "ERROR: " << message << ' ' << stringout << '\n';
		if (error == CG_COMPILER_ERROR)
			ss << cgGetLastListing(context) << '\n';
		throw std::exception(ss.str().c_str());
	}
}
