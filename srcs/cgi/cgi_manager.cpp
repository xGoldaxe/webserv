#include "cgi_manager.hpp"

inline static void          string_to_char(std::string to_convert, char **dest)
{
    *dest = new char[to_convert.length() + 1];
    std::strcpy(*dest, to_convert.c_str());
}

CGIManager::CGIManager(std::string root, std::string cgi_path, std::string path, int cgi_timeout)
{
    this->addHeader("GATEWAY_INTERFACE", "CGI/1.1");
    this->addHeader("SCRIPT_FILENAME", path);
    this->addHeader("DOCUMENT_ROOT", root);
    this->addHeader("SERVER_PROTOCOL", "HTTP/1.1");
    this->addHeader("SERVER_SOFTWARE", DEFAULT_SERVER_NAME);
    this->addHeader("REDIRECT_STATUS", "200");
    this->addHeader("PHP_SELF", path);

    this->_cgi_path = cgi_path;
    this->_path = path;
    this->_cgi_timeout = cgi_timeout;
    this->_sent_headers = false;
    this->_cgi_out_fd = -1;
    this->_out_chunk = "";

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

std::string CGIManager::exec(Request &req, std::string client_ip)
{
    std::string addr_port = req.get_header_value("Host");

    std::string addr = addr_port.substr(0, addr_port.find(":"));
    std::string port = addr_port.substr(addr_port.find(":") + 1);

    this->addHeader("PATH_INFO", req.get_legacy_url());
    this->addHeader("PATH_TRANSLATED", req.get_legacy_url());
    this->addHeader("SCRIPT_NAME", req.get_legacy_url());
    this->addHeader("CONTENT_LENGTH", to_string(req.getBody().length()));
    if (req.getMethod() == "GET") {
        this->addHeader("QUERY_STRING", req.get_query());
    } else {
        this->addHeader("QUERY_STRING", req.getBody());
    }
    this->addHeader("REMOTE_ADDR", client_ip);
    this->addHeader("REQUEST_METHOD", req.getMethod());
    this->addHeader("SERVER_NAME", addr);
    this->addHeader("SERVER_PORT", port);

    this->addHeader("HTTP_ACCEPT", req.get_header_value("Accept"));
    this->addHeader("HTTP_ACCEPT_CHARSET", req.get_header_value("Accept-Charset"));
    this->addHeader("HTTP_ACCEPT_ENCODING", req.get_header_value("Accept-Encoding"));
    this->addHeader("HTTP_ACCEPT_LANGUAGE", req.get_header_value("Accept-Language"));
    this->addHeader("HTTP_FORWARDED", req.get_header_value("Forwarded"));
    this->addHeader("HTTP_USER_AGENT", req.get_header_value("User-Agent"));
    this->addHeader("HTTP_HOST", addr_port);
    this->addHeader("HTTP_COOKIE", req.get_header_value("Cookie"));

    this->computeEnvArray();

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
        throw HTTPCode500();
    
    int pipe_in_fd[2];
    if (pipe(pipe_in_fd) == -1)
        throw HTTPCode500();

    char *exec_path;
    string_to_char(this->_cgi_path, &exec_path);
    char *args[] = {
        exec_path,
        NULL
    };

    int pid = fork();
    if (pid == -1) {
        delete [] exec_path;
        throw HTTPCode500();
    }
    if (pid == 0)
    {
        close(pipe_fd[0]);
        close(pipe_in_fd[1]);
        dup2(pipe_in_fd[0], STDIN_FILENO);
        dup2(pipe_fd[1], STDOUT_FILENO);

        execve(args[0], args, this->_c_headers);
        exit(2);
    }
    else
    {
        delete [] exec_path;

        std::string to_write = req.getBody();
        if (write(pipe_in_fd[1], to_write.c_str(), to_write.size()) < 0) {
            std::cerr << "[CGI][ERROR] can't transmit body informations." << std::endl;
        }
        close(pipe_in_fd[1]);
        close(pipe_fd[1]);

        this->_cgi_out_fd = pipe_fd[0];
        this->_pid = pid;

        this->_begin_response_time = std::time(0);
    }
    return "";
}

int CGIManager::readChunk(std::size_t chunk_size)
{
    // If we closed the cgi stdout
    if (this->_cgi_out_fd == 0) {
        return (CHUNK_OVER);
    }

    this->_out_chunk = this->_remaining;
    this->_remaining = "";
    
    struct pollfd fds;
    fds.fd = this->_cgi_out_fd;
    fds.events = POLLIN;

    while (!shouldQuit()) {
        if (poll(&fds, 1, 0) > 0) {
            if (!this->_sent_headers) {
                char temp;
                if (read(this->_cgi_out_fd, &temp, 1) < 1) {
                    this->_cgi_out_fd = 0;
                    break;
                }
                this->_out_chunk += temp;
                std::size_t pos_end_header = this->_out_chunk.find("\r\n\r\n");
                if (pos_end_header != this->_out_chunk.npos) {
                    this->_remaining = this->_out_chunk.substr(pos_end_header + std::string("\r\n\r\n").length());
                    this->_out_chunk = this->_out_chunk.substr(0, pos_end_header) + "\r\nTransfer-Encoding: chunked\r\n\r\n";
                    this->_sent_headers = true;
                    return (CHUNK_HEADER);
                }
            } else {
                this->_begin_response_time = std::time(0);

                char temp;
                int res = 0;
                if ((res = read(this->_cgi_out_fd, &temp, 1)) < 1) {
                    this->_cgi_out_fd = 0;
                    break;
                }
                this->_out_chunk += temp;
                if (this->_out_chunk.length() >= chunk_size) {
                    this->_remaining = this->_out_chunk.substr(chunk_size);
                    this->_out_chunk = this->_out_chunk.substr(0, chunk_size - 1);
                    return (CHUNK_CONTINUE);
                }
            }
        } else {
            if (std::time(0) - this->_begin_response_time < this->_cgi_timeout) {
                return (CHUNK_NEXT);
            } else {
                close(this->_cgi_out_fd);
                this->_cgi_out_fd = 0;
                kill(this->_pid, SIGABRT);
                throw HTTPCode504();
            }
        }
    }

    int status = 0;
    waitpid(this->_pid, &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 2) {
        std::cerr << "[CGI][Error] Unexisting CGI " << this->_cgi_path << std::endl;
        close(this->_cgi_out_fd);
        this->_cgi_out_fd = -1;
        throw HTTPCode502();
    } else if (WIFSIGNALED(status) && WTERMSIG(status) == SIGALRM) {
        std::cerr << "[CGI][Error] CGI Timeout " << this->_cgi_path << std::endl;
        close(this->_cgi_out_fd);
        this->_cgi_out_fd = -1;
        throw HTTPCode504();
    }
    if (this->_out_chunk.length() == 0 && this->_sent_headers == false) {
        std::cerr << "[CGI][Error] Unvalid response for " << this->_cgi_path << std::endl;
        close(this->_cgi_out_fd);
        this->_cgi_out_fd = -1;
        throw HTTPCode502();
    }

    // close(this->_cgi_out_fd);
    // this->_cgi_out_fd = 0;

    this->_out_chunk += this->_remaining;
    this->_remaining = "";

    if (this->_out_chunk != "") {
        return (CHUNK_CONTINUE);
    }
    return (CHUNK_NEXT);
}

std::string    CGIManager::getOutput() const
{
    return this->_out_chunk;
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
