#include "exception_server_not_listening.hpp"

const char *ServerNotListeningException::what() const throw()
{
    return "Server is not listening on specified port.";
}