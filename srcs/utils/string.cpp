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

std::string get_extension(std::string path)
{
    std::string filename = get_filename(path);
    int position = filename.find_last_of(".");
    return filename.substr(position + 1);
}

std::string get_filename(std::string file_name)
{
    int position = file_name.find_last_of("/");
    return file_name.substr(position + 1);
}

std::string intToHex(int nb)
{
    std::stringstream stream;
    stream << std::hex << nb;
    std::string result(stream.str());
    return result;
}

std::string ulToStr(std::size_t nb)
{
    std::stringstream stream;
    stream << nb;
    std::string result(stream.str());
    return result;
}
