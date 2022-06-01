#pragma once
#include <exception>
#include <string>
#include "error_code_http.hpp"

class ErrorHTTP5xx : public ErrorCodeHTTP
{
public:
    ErrorHTTP5xx(int code, std::string error) throw() : ErrorCodeHTTP(5, code, error) {}
};

class ErrorHTTP500 : public ErrorHTTP5xx
{
public:
    ErrorHTTP500() throw() : ErrorHTTP5xx(0, "Internal Server Error") {}
};
