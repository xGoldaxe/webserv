#include "exception_content_type.hpp"

const char *ExceptionContentType::what() const throw()
{
    return "The content type is not well formated.";
}