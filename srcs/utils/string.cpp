#include "string.hpp"

std::string finish_by_only_one(std::string str, char c)
{

    if (str.size() == 0)
        return str;
    if (str.at(str.size() - 1) == c)
        return str;
    str += c;
    return str;
}

std::string get_extension(std::string file_name)
{
    int position = file_name.find_last_of(".");
    return file_name.substr(position + 1);
}
