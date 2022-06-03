#include "cgi_manager.hpp"

inline static char *string_to_char(std::string to_convert)
{
    char *cstr = new char[to_convert.length() + 1];
    std::strcpy(cstr, to_convert.c_str());
    return cstr;
}

CGIManager::CGIManager(t_mime_list mimes, std::string cgi_path) : _mime_types(mimes)
{
    /** @todo change this */
    this->addHeader("PATH_INFO", "/mnt/nfs/homes/tbelhomm/Desktop/webserv/www");
    this->addHeader("GATEWAY_INTERFACE", "CGI/1.1");
    this->addHeader("QUERY_STRING", "");
    this->addHeader("REMOTE_ADDR", "");
    this->addHeader("REQUEST_METHOD", "");
    this->addHeader("SCRIPT_NAME", "");
    this->addHeader("SERVER_NAME", "localhost");
    this->addHeader("SERVER_PORT", "3000");
    this->addHeader("SERVER_PROTOCOL", "HTTP/1.1");
    this->addHeader("SERVER_SOFTWARE", "webserv-1.0");
    this->addHeader("REDIRECT_STATUS", "200");
    this->addHeader("PATH_INFO", cgi_path);
    this->addHeader("SCRIPT_FILENAME", "");

    this->_cgi_path = cgi_path;

#ifdef DEBUG
                          std::cout
                      << "New CGI for following MIME:" << std::endl;
        for (t_mime_list::iterator it = mimes.begin(); it != mimes.end(); it++) {
            std::cout << *it << std::endl;
        }
    #endif
}

CGIManager::~CGIManager()
{
    
}

std::string CGIManager::exec(Request &req)
{
    #ifdef DEBUG
        std::cout << "CGI " << req.get_route().cgi_path.c_str() << " used for " << req.getUrl().c_str() << std::endl;
    #endif

    this->addHeader("CONTENT_LENGTH", to_string(req.getBody().length()));
    this->addHeader("QUERY_STRING", req.get_legacy_url());
    this->addHeader("REMOTE_ADDR", "127.0.0.1");
    this->addHeader("REQUEST_METHOD", req.getMethod());
    this->addHeader("SCRIPT_NAME", req.get_route().cgi_path);
    this->addHeader("PATH_INFO", req.get_route().root);
    this->addHeader("SCRIPT_FILENAME", req.getUrl());

    this->computeEnvArray();

    std::string result;

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
        throw HTTPCode500();

    char *args[] = {
        string_to_char(this->_cgi_path),
        NULL
    };

    int pid = fork();
    if (pid == -1) {
        delete args[0];
        throw HTTPCode500();
    }
    if (pid == 0)
    {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);

        execve("/usr/bin/php-cgi", args, &this->_c_headers[0]);
        exit(2);
    }
    else
    {
        delete args[0];

        close(pipe_fd[1]);
        result = read_fd(pipe_fd[0]);

        #ifdef DEBUG
            std::cout << result << std::endl;
        #endif

        int status = 0;
        waitpid(pid, &status, 0);

        if (status != 0)
            throw HTTPCode500();
        close(pipe_fd[0]);
    }

    return result;
}

void CGIManager::addHeader(std::string name, std::string value)
{
    this->_headers[name] = value;
}

void CGIManager::computeEnvArray()
{
    for (std::vector<char *>::iterator it = this->_c_headers.begin(); it != this->_c_headers.end(); it++) {
        delete [] *it;
        this->_c_headers.erase(it);
    }

    for (t_header_value::iterator it = this->_headers.begin(); it != this->_headers.end(); it++) {
        this->_c_headers.push_back(string_to_char(it->first + "=" + it->second));
    }
}
