#pragma once

#include <string>
#include <sstream>

template <typename T>
std::string to_string(T convert)
{
    std::ostringstream stream;
    stream << convert;
    return (stream.str());
}

std::string finish_by_only_one(std::string str, char c);
std::string get_filename(std::string file_name);
std::string get_extension(std::string file_name);
std::string intToHex(int nb);