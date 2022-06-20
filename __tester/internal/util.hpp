#pragma once

#include <string>
#include <sstream>

template <typename T>
std::string to_string_int(T convert)
{
    std::ostringstream stream;
    stream << convert;
    return (stream.str());
}