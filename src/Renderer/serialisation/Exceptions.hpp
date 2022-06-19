#pragma once

#include <iomanip>
#include <stdio.h>
#include <sstream>

namespace serialisation
{
    // BadFilePointerException
    class BadFilePointerException : public std::runtime_error
    {
    public:
        BadFilePointerException(int expected_pos, int actual_pos);
        const char* what() const noexcept override;
    private:
        int expected_pos;
        int actual_pos;
    };

    // NotAtEOfException
    class NotAtEOFException : public std::runtime_error
    {
    public:
        NotAtEOFException(int pos);
        const char* what() const noexcept override;
    private:
        int pos;
    };


    // UnexpectedValueException
    template<typename T>
    class UnexpectedValueException : public std::runtime_error
    {
    public:
        UnexpectedValueException<T>(const std::string& variable_name, T expected_value, T actual_value);
        const char* what() const noexcept override;
    private:
        std::string variable_name;
        T expected_value;
        T actual_value;
    };

    template<typename T>
    UnexpectedValueException<T>::UnexpectedValueException(const std::string& variable_name, T expected_value, T actual_value) : std::runtime_error("<UnexpectedValueException: THIS SHOULD NOT DISPLAY>")
    {
        UnexpectedValueException::variable_name = variable_name;
        UnexpectedValueException::expected_value = expected_value;
        UnexpectedValueException::actual_value = actual_value;
    }

    template<typename T>
    const char* UnexpectedValueException<T>::what() const noexcept
    {

        std::stringstream message;
        message << "Expected '" << variable_name << "' to be " << expected_value << ", not " << actual_value << ".";
        return message.str().c_str();
    }

    // InconsistentFilePointerAndCountException
    template<typename T>
    class InconsistentFilePointerAndCountException : public std::runtime_error
    {
    public:
        InconsistentFilePointerAndCountException<T>(const std::string& ptr_name, T ptr_value, const std::string& count_name, T count_value);
        const char* what() const noexcept override;
    private:
        std::string ptr_name;
        T ptr_value;
        std::string count_name;
        T count_value;
    };

    template<typename T>
    InconsistentFilePointerAndCountException<T>::InconsistentFilePointerAndCountException(const std::string& ptr_name, T ptr_value, const std::string& count_name, T count_value) : std::runtime_error("<InconsistentFilePointerAndCountException: THIS SHOULD NOT DISPLAY>")
    {
        InconsistentFilePointerAndCountException::ptr_name = ptr_name;
        InconsistentFilePointerAndCountException::ptr_value = ptr_value;
        InconsistentFilePointerAndCountException::count_name = count_name;
        InconsistentFilePointerAndCountException::count_value = count_value;
    }

    template<typename T>
    const char* InconsistentFilePointerAndCountException<T>::what() const noexcept
    {

        std::stringstream message;
        message << "Pointer '" << ptr_name << "' has a value of " << ptr_value
            << ", and count '" << count_name << "' has a value of " << count_value
            << ". Both should either be zero or non-zero.";
        return message.str().c_str();
    }
}
