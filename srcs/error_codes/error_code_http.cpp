#include "error_code_http.hpp"

ErrorCodeHTTP::ErrorCodeHTTP(int section, int code, std::string error) throw() : http_error_section(1), http_error_code(0), error_description(error)
{
    if (section < 1 || section > 5)
        throw new std::logic_error("Impossible HTTP Request " + to_string(code));
    if (code < 0 || code > 99)
        throw new std::logic_error("Impossible HTTP Request " + to_string(code));
    this->http_error_section = section;
    this->http_error_code = code;
}

ErrorCodeHTTP::ErrorCodeHTTP(const ErrorCodeHTTP &rhs) throw() : http_error_section(rhs.http_error_section), http_error_code(rhs.http_error_code), error_description(rhs.error_description)
{}

ErrorCodeHTTP &ErrorCodeHTTP::operator=(const ErrorCodeHTTP &rhs) throw()
{
    if (this == &rhs)
        return *this;
    this->http_error_section = this->getCode() / 100;
    this->http_error_code = this->getCode() - this->http_error_section;
    this->error_description = "";
    return *this;
}

ErrorCodeHTTP::~ErrorCodeHTTP() throw()
{}

const char *ErrorCodeHTTP::what() const throw()
{
    std::string res = to_string((this->http_error_section * 100) + this->http_error_code);
    res += ": " + this->error_description;
    return res.c_str();
}

int ErrorCodeHTTP::getCode() const
{
    return (this->http_error_section * 100) + this->http_error_code;
}

std::string ErrorCodeHTTP::getError() const
{
    return this->error_description;
}
