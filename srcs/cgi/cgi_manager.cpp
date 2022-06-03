#include "cgi_manager.hpp"

inline static void string_to_char(std::string to_convert, char **dest)
{
    *dest = new char[to_convert.length() + 1];
    std::strcpy(*dest, to_convert.c_str());
}

CGIManager::CGIManager(std::string cgi_path, std::string path)
{
    /** @todo change this */
    this->addHeader("PATH_INFO", path);

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

    this->_c_headers = new char* [1];
    this->_c_headers[0] = NULL;

    #ifdef DEBUG
        std::cout << "[CGI] New with " << cgi_path << " for " << path << std::endl;
    #endif
}

CGIManager::~CGIManager()
{
    this->cleanCHeaders();
}

std::string CGIManager::exec(Request &req)
{
    this->addHeader("CONTENT_LENGTH", to_string(req.getBody().length()));
    this->addHeader("QUERY_STRING", req.get_legacy_url());
    this->addHeader("REMOTE_ADDR", "127.0.0.1");
    this->addHeader("REQUEST_METHOD", req.getMethod());
    this->addHeader("SCRIPT_NAME", req.get_route().get_cgi_path());
    this->addHeader("PATH_INFO", req.get_route().get_root());
    this->addHeader("SCRIPT_FILENAME", req.getUrl());

    this->computeEnvArray();

    std::string result;

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
        throw HTTPCode500();

    char *arg;
    string_to_char(this->_cgi_path, &arg);
    char *args[] = {
        arg,
        NULL
    };

    int pid = fork();
    if (pid == -1) {
        delete [] arg;
        throw HTTPCode500();
    }
    if (pid == 0)
    {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);

        execve(args[0], args, this->_c_headers);
        exit(2);
    }
    else
    {
        close(pipe_fd[1]);
        result = read_fd(pipe_fd[0]);

        #ifdef DEBUG_FULL
            std::cout << "request content: " << result << std::endl;
        #endif

        int status = 0;
        waitpid(pid, &status, 0);

        close(pipe_fd[0]);

        if (status != 0) {
            std::cerr << "500: \"" << result << "\"" << std::endl;
            throw HTTPCode500();
        }
    }

    delete [] arg;
    return result;
}

void CGIManager::addHeader(std::string name, std::string value)
{
    this->_headers[name] = value;
}

void CGIManager::computeEnvArray()
{
    // Free previous headers
    this->cleanCHeaders();

    // Allocate memory size
    this->_c_headers = new char *[this->_headers.size() + 1];
    int i = 0;

    for (t_header_value::iterator it = this->_headers.begin(); it != this->_headers.end(); it++)
    {
        std::string res = (it->first + "=" + it->second);

        this->_c_headers[i] = new char[res.length() + 1];
        std::strcpy(this->_c_headers[i++], res.c_str());
    }
    this->_c_headers[i] = NULL;
}

void CGIManager::cleanCHeaders()
{
    for (int i = 0; this->_c_headers[i] != NULL; i++)
    {
        delete [] this->_c_headers[i];
    }
    delete [] this->_c_headers;
}
