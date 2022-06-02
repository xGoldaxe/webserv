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

/*************************
* @5xx
* ***********************/

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

/*************************
* @4xx
* ***********************/

class HTTPCode4XX : public HTTPError
{
    public:
    HTTPCode4XX(int code, std::string error) throw() : HTTPError(4, code, error)
    {}
};

class HTTPCode403 : public HTTPCode4XX
{
    public:
    HTTPCode403() throw() : HTTPCode4XX(3, "Forbidden")
    {}
};

class HTTPCode404 : public HTTPCode4XX
{
    public:
    HTTPCode404() throw() : HTTPCode4XX(4, "Not Found")
    {}
};