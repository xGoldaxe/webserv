#include "http_code.hpp"

HTTPError::HTTPError(int section, int code, std::string error) throw() : std::exception(), section_code(section), error_code(code), error(error)
{
    this->generateResponseString();
}

HTTPError::HTTPError(const HTTPError &rhs) throw() : std::exception(), section_code(rhs.section_code), error_code(rhs.error_code), error(rhs.error)
{
    delete [] this->error_string;
    this->generateResponseString();
}

HTTPError& HTTPError::operator=(const HTTPError &rhs) throw()
{
    if (this == &rhs)
        return (*this);
    this->section_code = rhs.getSectionCode();
    this->error_code = rhs.getErrorCode();
    this->error = rhs.getDescription();
    delete [] this->error_string;
    this->generateResponseString();
    return *this;
}

HTTPError::~HTTPError() throw()
{
    delete [] this->error_string;
}

const char* HTTPError::what() const throw()
{
    return this->error_string;
}

int HTTPError::getSectionCode() const
{
    return this->section_code;
}

int HTTPError::getErrorCode() const
{
    return this->error_code;
}

std::string HTTPError::getDescription() const
{
    return this->error;
}

int HTTPError::getCode() const
{
    return (this->getSectionCode() * 100) + this->getErrorCode();
}

void    HTTPError::generateResponseString()
{
    std::string err = to_string(this->getCode()) + ": " + this->getDescription();
    this->error_string = new char[err.length()];
    memcpy(this->error_string, err.c_str(), err.length());
}
