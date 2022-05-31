#pragma once

#include <exception>

class ServerNotListeningException : virtual public std::exception
{
    const char *what() const throw();
};
