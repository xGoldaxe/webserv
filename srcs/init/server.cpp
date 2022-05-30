#include "server.hpp"

Server::Server()
{
    this->_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    this->_addr.sin_family = AF_INET;
    this->_addr.sin_port = htons(3000);
    this->_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    this->_port = this->_select_port();

    if (listen(this->_socket_fd, BACKLOG) < 0)
        throw new ServerNotListeningException;

    this->_report(&this->_addr);
}

Server::~Server()
{
    std::cout << "Server closed." << std::endl;
}

int Server::get_socket() const
{
    return this->_socket_fd;
}

short Server::_select_port()
{
    while (bind(this->_socket_fd, (s_server_addr)&this->_addr, sizeof(this->_addr)) == -1)
    {
        this->_addr.sin_port = htons(++this->_port);
        #ifdef DEBUG
            std::cout << this->_port << std::endl;
        #endif
    }

    return this->_port;
}

void Server::_report(s_server_addr_in *server_addr)
{
    char host_buffer[INET6_ADDRSTRLEN];
    char service_buffer[NI_MAXSERV];
    socklen_t addr_len = sizeof(*server_addr);
    int err = getnameinfo(
        (s_server_addr)server_addr,
        addr_len,
        host_buffer,
        sizeof(host_buffer),
        service_buffer,
        sizeof(host_buffer),
        NI_NUMERICHOST);
    if (err != 0)
    {
        std::cout << "It's not working!" << std::endl;
    }
    std::cout << "\n\n\tServer listening on http://" << host_buffer << ":" << service_buffer << std::endl;
}
