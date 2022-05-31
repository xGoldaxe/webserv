#pragma once

#include <exception>

class ExceptionContentType : virtual public std::exception
{
    const char *what() const throw();
};
