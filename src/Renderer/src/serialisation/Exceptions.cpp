#include "Exceptions.hpp"

namespace serialisation 
{
    BadFilePointerException::BadFilePointerException(int expected_pos, int actual_pos) : std::runtime_error("<Bad File Pointer Exception: THIS SHOULD NOT DISPLAY>")
    {
        BadFilePointerException::expected_pos = expected_pos;
        BadFilePointerException::actual_pos = actual_pos;
    }

    const char* BadFilePointerException::what() const noexcept
    {

        std::stringstream message;
        message << "Expected file pointer to be at position "
            << std::hex << expected_pos
            << " (dec: " << std::to_string(expected_pos) << "), was at "
            << std::hex << actual_pos
            << " (dec: " << std::to_string(actual_pos) << ").";
        return message.str().c_str();
    }


    NotAtEOFException::NotAtEOFException(int pos) : std::runtime_error("<Bad File Pointer Exception: THIS SHOULD NOT DISPLAY>")
    {
        NotAtEOFException::pos = pos;
    }

    const char* NotAtEOFException::what() const noexcept
    {

        std::stringstream message;
        message << "Expected file pointer to be at end of file, was at "
            << std::hex << pos
            << " (dec: " << std::to_string(pos) << ").";
        return message.str().c_str();
    }

}
