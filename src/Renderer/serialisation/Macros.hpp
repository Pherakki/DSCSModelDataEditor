#pragma once

#include "Exceptions.hpp"

namespace serialisation
{
#define NAME_OF(variable) #variable
#define VALUE_AND_NAME_OF(variable) variable, NAME_OF(variable)
#define ASSERT_VALUE_EQUALS_CONST(value_to_check, const_to_check_against)                                                      \
    {                                                                                                                              \
        if (value_to_check != const_to_check_against)                                                                              \
        {                                                                                                                          \
            throw UnexpectedValueException<std::string>(NAME_OF(const_to_check_against), const_to_check_against, value_to_check);  \
        }                                                                                                                          \
    }
}
