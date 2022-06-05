#pragma once

#include <cstring>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h> // for getnameinfo()
#include <fcntl.h>

// Usual socket headers
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <netinet/tcp.h> /* TCP_KEEPIDLE, etc. */
#include <arpa/inet.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sys/epoll.h>
#include <sys/stat.h>

#include <queue>

#include "exception_server_not_listening.hpp"
#include "class/response.hpp"

#define BACKLOG 10

typedef struct sockaddr_in s_server_addr_in;
typedef const struct sockaddr* s_server_addr;

class Server {
    public:
        Server();
        ~Server();
        void    init_connection();
        void    handle_client();

        // Getters
        int     get_socket() const;
        int     get_poll_fd() const;

        bool    queue_response(Response *res);
        void    handle_responses();

    private:
        short                   _port;
        s_server_addr_in        _addr;
        int                     _socket_fd;
        int                     _poll_fd;
        std::queue<Response *>   _queue;

        void     _report(s_server_addr_in *server_addr);
        void     _bind_port();
};
