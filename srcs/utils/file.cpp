#include "file.hpp"

bool file_exist(const std::string &name)
{
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

int is_file(std::string dir)
{
    const char *dirname = dir.c_str();
    DIR *directory = opendir(dirname);

    if (directory != NULL)
    {
        closedir(directory);
        return 0;
    }

    std::ifstream File( dir.c_str() );
    if ( File.is_open() == true )
    {
        File.close();
        return 1;
    }
    // doesnt exist
    return -1;
}

bool usable_file(const std::string &name)
{
    return (file_exist(name) && is_file(name.c_str()) == 1 && file_readable(name));
}

bool file_readable(const std::string &name)
{
    std::ifstream my_file(name.c_str());
    return (my_file.good());
}