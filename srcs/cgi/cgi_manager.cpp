#include "cgi_manager.hpp"

CGIManager::CGIManager(t_mime_list mimes) : _mime_types(mimes)
{
    this->setRequiredHeadersName();
    this->fillServerInfos();
    /** @todo change this */
    this->_headers["PATH_INFO"] = "/mnt/nfs/homes/tbelhomm/Desktop/webserv/www";

    #ifdef DEBUG
        std::cout << "New CGI for following MIME:" << std::endl;
        for (t_mime_list::iterator it = mimes.begin(); it != mimes.end(); it++) {
            std::cout << *it << std::endl;
        }
    #endif
}

void CGIManager::exec(Request &req)
{
    (void)req;
    // this->_headers["QUERY_STRING"] = req.;
    // this->_headers["CONTENT_LENGTH"] = to_string( req.body.size() );


    // int pipe_fd[2];
    // pipe(pipe_fd);
    // int pid = fork();

    // if (pid == 0) {
    //     close( pipe_fd[0] );
    //     dup2( pipe_fd[1], STDOUT_FILENO );
    //     char *args[3];

    //     for (char **env = req.env; *env != 0; env++)
    //     {
    //         char *thisEnv = *env;
    //         printf("%s\n", thisEnv);    
    //     }

    //     args[0] = const_cast<char *> ( req.get_route().cgi_path.c_str() );
    //     args[1] = const_cast<char *> ( req.getUrl().c_str() );
    //     args[2] = NULL;
    //     execve(req.get_route().cgi_path.c_str(), args, req.env);
    //     exit(2);
    // }

    // close(pipe_fd[1]);
    // int status = 0;
    // waitpid(pid, &status, 0);
    
    // if (status != 0) {
    //     this->set_status( 500, "Internal Server Error" );
    //     this->error_body();
    // } else {
    //     this->add_header( "Content-Type", "text/html" );
    //     new_body = read_fd(pipe_fd[0]);
    //     close(pipe_fd[0]);
    // }
}

void CGIManager::setRequiredHeadersName(void)
{
    t_headers_list headers;

    headers.push_back("CONTENT_LENGTH");
    headers.push_back("CONTENT_TYPE");
    headers.push_back("GATEWAY_INTERFACE"); //
    headers.push_back("PATH_INFO"); //
    headers.push_back("QUERY_STRING"); //
    headers.push_back("REMOTE_ADDR");
    headers.push_back("REQUEST_METHOD");
    headers.push_back("SCRIPT_NAME");
    headers.push_back("SERVER_NAME"); //
    headers.push_back("SERVER_PORT"); //
    headers.push_back("SERVER_PROTOCOL"); //
    headers.push_back("SERVER_SOFTWARE"); //

    for (t_headers_list::iterator it = headers.begin(); it != headers.end(); it++) {
        this->_headers.insert(std::make_pair(*it, std::string("")));
    }

    return;
}

void CGIManager::fillServerInfos()
{
    this->_headers["GATEWAY_INTERFACE"] = "CGI/1.1";
    this->_headers["SERVER_NAME"] = "localhost";
    this->_headers["SERVER_PORT"] = "3000";
    this->_headers["SERVER_PROTOCOL"] = "HTTP/1.1";
    this->_headers["SERVER_SOFTWARE"] = WEBSERV_VERSION;
}
