#include "init/server.hpp"

#include <strings.h>

#include "webserv.hpp"

#define SIZE 1024

/* throw a server exeption in case of failure */

void process_request( std::string raw_request, char **env)
{
	// char buffer[256];
	// /* with MSG_PEEK, no data will be ride of the socket */
	// if ( recv(client_socket, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT) == 0 )
	// {
	// 	std::cout << "Client close remote: " << client_socket << std::endl;
	// 	close( client_socket );
	// 	return ;
	// }

	(void) raw_request;
	(void) env;
	return ;
	// /* we will need further verification */
	
}

MimeTypes mimes;

//testing confparser if you type ./webserv testconf
//to remove when done
void testconf()
{
	
	try
	{
		Webserv_conf conf = Webserv_conf("idOnotExist");
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}

	try
	{
		Webserv_conf conf = Webserv_conf("./config/default.wbserv");
		conf.getServers()[0].printServer();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}

}

int main(int argc, char **argv, char **env)
{
	(void) argc;
	(void) argv;

	if(argc == 2 && strcmp(argv[1],"testconf") == 0)
	{
		testconf();
		return(0);
	}

	mimes.setDefault();

	/************************************************************************
	 * Example of working mimes parsing                                     */
	std::cout << mimes.getMimeForExtension("html") << std::endl;
	try {
		std::cout << mimes.getMimeForExtension("inconnu") << std::endl;
	} catch (MimeType::ExceptionUnknownMimeType *e) {
		std::cout << e->what() << std::endl;
	}

	mimes.parseHTTP("Content-Type = text/html");
	mimes.parseHTTP("Content-Type = text/html; charset=utf-8");
	mimes.parseHTTP("Content-Type = text/html ;Charset=utf-8");
	mimes.parseHTTP("Content-Type = text/html ; charset=utf-8");

	/* End of Example                                                       *
	*************************************************************************/

	Server serv = Server();
	serv.init_connection();

	(void)env;
	while (true) {
		serv.handle_client();
		serv.wait_for_connections();
	}
	/* connections must have a lifetime */
	return 0;
}
