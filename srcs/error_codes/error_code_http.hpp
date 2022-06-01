#pragma once

#include "../webserv.hpp"
#include <exception>
#include <string>

class ErrorCodeHTTP : public std::exception
{
public:
    ErrorCodeHTTP(int section, int code, std::string error) throw();
    ErrorCodeHTTP(const ErrorCodeHTTP &rhs) throw();
    ErrorCodeHTTP &operator=(const ErrorCodeHTTP &rhs) throw();
    virtual ~ErrorCodeHTTP() throw();
    virtual const char *what() const throw();
    int getCode() const;
    std::string getError() const;

private:
    int http_error_section;
    int http_error_code;
    ErrorCodeHTTP() throw();
    std::string error_description;
};