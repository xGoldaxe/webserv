#include "cgi_manager.hpp"

inline static void          string_to_char(std::string to_convert, char **dest)
{
    *dest = new char[to_convert.length() + 1];
    std::strcpy(*dest, to_convert.c_str());
}

inline static std::string   read_fd(int fd)
{
	char buff[256];
	int readed = 255;
	std::string	res;
	while (readed != 0)
	{
		std::memset(buff, 0, 256);
		readed = read( fd, buff, 255 );
		buff[readed] = '\0';
		res += buff;
	}
	return res;
}

CGIManager::CGIManager(std::string root, std::string cgi_path, std::string path)
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

    this->addHeader("PATH_INFO", get_filename(this->_path));
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

    std::string result;

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
        std::string to_write = req.getBody();
        std::cout << "CGI Parameters: " << to_write << std::endl;
        if (write(pipe_in_fd[1], to_write.c_str(), to_write.size()) < 0) {
            std::cerr << "[CGI][ERROR] can't transmit body informations." << std::endl;
        }
        close(pipe_in_fd[1]);
        close(pipe_fd[1]);
        while (1) {
            char temp;
            if (read(pipe_fd[0], &temp, 1) < 1) break;
            result += temp;
        }

        int status = 0;
        waitpid(pid, &status, 0);

        close(pipe_fd[0]);
    }

    delete [] exec_path;
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
