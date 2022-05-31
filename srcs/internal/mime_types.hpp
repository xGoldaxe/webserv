#pragma once

#include <string>
#include <map>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <exception>

#include "exception_content_type.hpp"

class ExceptionUnknownMimeType : virtual public std::exception
{
    const char *what() const throw()
    {
        return "extension inconnue.";
    };
};

class MimeType {
public:
    MimeType(const std::string name, const std::string types, const std::string extension);
    MimeType(const MimeType &cpy);

    MimeType &operator=(MimeType &cpy);

    std::string getContentType(void);

    friend std::ostream &operator<<(std::ostream &os, const MimeType &dt);

private:
    const std::string _name;
    const std::string _extension;
    const std::string _types;
};

class MimeTypes {
public:
    typedef std::map<std::string, MimeType> _map_extensions_type;
    typedef std::pair<std::string, MimeType> _map_pair_type;

    MimeTypes();
    MimeTypes(const MimeTypes &cpy);
    MimeTypes &operator=(const MimeTypes &cpy);

    ~MimeTypes();

    void setDefault();

    void parseHTTP(std::string req);

    void addMimeType(const std::string type, const std::string name, const std::string extension);
    void addMimeType(const std::string type, const std::string name, const char *extensions[]);

    MimeType getMimeForExtension(std::string extension);

    friend std::ostream &operator<<(std::ostream &os, const MimeTypes &dt);

private:
    // Extensions map to type
    _map_extensions_type _map_extensions;
};
