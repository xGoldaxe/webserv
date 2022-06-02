#pragma once

#include <exception>
#include <string>
#include <cstring>
#include "../webserv.hpp"

class HTTPError : public std::exception
{
    public:
    HTTPError(int section, int code, std::string error) throw();
    HTTPError(const HTTPError&) throw();
    HTTPError& operator=(const HTTPError&) throw();
    virtual ~HTTPError() throw();
    virtual const char* what() const throw();

    int getSectionCode() const;
    int getErrorCode() const;
    std::string getDescription() const;
    int getCode() const;

    private:
    int section_code;
    int error_code;
    std::string error;
    char *error_string;

    HTTPError () throw();
    void    generateResponseString();
};

class HTTPCode5XX : public HTTPError
{
    public:
    HTTPCode5XX(int code, std::string error) throw() : HTTPError(5, code, error)
    {}
};

class HTTPCode500 : public HTTPCode5XX
{
    public:
    HTTPCode500() throw() : HTTPCode5XX(0, "Internal Server Error")
    {}
};

