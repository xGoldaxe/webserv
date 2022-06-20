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
    this->_pid = 0;
    this->_sent_headers = false;
    this->_out_chunk = "";

    this->_c_headers = NULL;

    #ifdef DEBUG
        std::cout << "[CGI] New with " << cgi_path << " for " << path << std::endl;
    #endif
}

CGIManager::~CGIManager()
{
    this->cleanCHeaders();
    this->killCGI();
}

void CGIManager::killCGI()
{
    if (this->_pid > 0)
    {
        kill(this->_pid, SIGKILL);
        this->_pid = 0;
    }
}

std::string CGIManager::exec(Request &req, std::string client_ip)
{
    std::string addr_port = req.get_header_value("Host");

    std::string addr = addr_port.substr(0, addr_port.find(":"));
    std::string port = addr_port.substr(addr_port.find(":") + 1);

    this->addHeader("PATH_INFO", req.get_legacy_url());
    this->addHeader("PATH_TRANSLATED", req.get_legacy_url());
    this->addHeader("SCRIPT_NAME", req.get_legacy_url());
    this->addHeader("CONTENT_LENGTH", to_string(req.get_body_content().length()));
    this->addHeader("CONTENT_TYPE", req.get_header_value("Content-Type"));
    this->addHeader("QUERY_STRING", req.get_query());
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
        std::cerr << "[CGI][FATAL] Crashed!!" << std::endl;
        throw HTTPCode500();
    }
    if (pid == 0)
    {
        close(pipe_in_fd[1]);
        close(pipe_fd[0]);

        dup2(pipe_in_fd[0], STDIN_FILENO);
        dup2(pipe_fd[1], STDOUT_FILENO);

        execve(args[0], args, this->_c_headers);
        exit(2);
    }
    else
    {
        delete [] exec_path;

        close(pipe_in_fd[0]);
        close(pipe_fd[1]);

        // Write the body content
        std::string to_write = req.get_body_content();
        if (to_write.size() > 0) {
            if (write(pipe_in_fd[1], to_write.c_str(), to_write.size()) <= 0) {
                std::cerr << "[CGI][ERROR] can't transmit body informations." << std::endl;
                this->killCGI();
                throw HTTPCode502();
            }
        }
        close(pipe_in_fd[1]);

        this->_begin_response_time = std::time(0);

        this->_sent_headers = false;
        this->_cgi_out_fd = pipe_fd[0];
        this->_pid = pid;
        this->_out_chunk = "";
        this->_remaining = "";

        if (this->_cgi_out_fd <= 0) {
            std::cerr << "[CGI][ERROR] Invalid output." << std::endl;
            this->killCGI();
            throw HTTPCode502();
        }
    }
    return "";
}

int CGIManager::readChunk(std::size_t chunk_size)
{
    // If we closed the cgi stdout
    if (this->_cgi_out_fd == 0) {
        this->_out_chunk = this->_remaining;
        this->_remaining = "";
        return (CHUNK_OVER);
    }

    struct pollfd fds;
    fds.fd = this->_cgi_out_fd;
    fds.events = POLLIN;
    fds.revents = 0;

    while (!shouldQuit()) {
        int updates = poll(&fds, 1, 0);
        if (updates > 0 && fds.revents & (POLLIN|POLLHUP)) {
            if (!this->_sent_headers) {
                char temp;
                int res = 0;
                if ((res = read(this->_cgi_out_fd, &temp, 1)) <= 0) {
                    close(this->_cgi_out_fd);
                    this->_cgi_out_fd = 0;
                    fds.fd = -1;
                    if (res < 0)
                        this->killCGI();
                    break;
                }
                this->_out_chunk += temp;
                std::size_t pos_end_header = this->_out_chunk.find("\r\n\r\n");
                if (pos_end_header != this->_out_chunk.npos) {
                    this->_remaining = this->_out_chunk.substr(pos_end_header + std::string("\r\n\r\n").length());

                    this->_out_chunk.erase(this->_out_chunk.begin() + pos_end_header, this->_out_chunk.end());
                    this->_out_chunk += "\r\nTransfer-Encoding: chunked\r\n\r\n";
                    this->_sent_headers = true;
                    return (CHUNK_HEADER);
                }
            } else {
                this->_begin_response_time = std::time(0);

                char temp;
                int res = 0;
                if ((res = read(this->_cgi_out_fd, &temp, 1)) <= 0) {
                    close(this->_cgi_out_fd);
                    this->_cgi_out_fd = 0;
                    fds.fd = -1;
                    if (res < 0)
                        this->killCGI();
                    break;
                }
                this->_out_chunk += temp;
                if (this->_out_chunk.length() >= chunk_size) {
                    this->_remaining = this->_out_chunk.substr(chunk_size);
                    this->_out_chunk = this->_out_chunk.substr(0, chunk_size);
                    return (CHUNK_CONTINUE);
                }
            }
        } else if (updates < 0) {
            close(this->_cgi_out_fd);
            this->_cgi_out_fd = 0;
            this->killCGI();
            throw HTTPCode500();
        } else if (!this->_sent_headers) {
            if (std::time(0) - this->_begin_response_time < this->_cgi_timeout) {
                return (CHUNK_NEXT);
            } else {
                close(this->_cgi_out_fd);
                this->_cgi_out_fd = 0;
                this->killCGI();
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
    }
    if (this->_out_chunk.length() == 0 && !this->_sent_headers) {
        std::cerr << "[CGI][Error] Invalid response for " << this->_cgi_path << std::endl;
        close(this->_cgi_out_fd);
        this->_cgi_out_fd = -1;
        throw HTTPCode500();
    }
    if (this->_out_chunk == "")
        return (CHUNK_NEXT);
    return (CHUNK_CONTINUE);
}

std::string    CGIManager::getOutput()
{
    std::string out = this->_out_chunk;
    this->_out_chunk =  this->_remaining;
    return out;
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
    if (this->_c_headers == NULL)
        return;
    for (int i = 0; this->_c_headers[i] != NULL; i++)
    {
        delete [] this->_c_headers[i];
    }
    delete [] this->_c_headers;
    this->_c_headers = NULL;
}
