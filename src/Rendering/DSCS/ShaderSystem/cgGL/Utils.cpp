#include "Utils.hpp"

// checkForCgError
const void checkForCgError(const CGcontext& context, const std::string& message)
{
	CGerror error;
	const char* stringout = cgGetLastErrorString(&error);

	if (error != CG_NO_ERROR)
	{
		std::cout << "ERROR: " << message << ' ' << stringout << '\n' << std::endl;
		if (error == CG_COMPILER_ERROR)
			std::cout << cgGetLastListing(context) << '\n' << std::endl;
		exit(1);
	}
}
