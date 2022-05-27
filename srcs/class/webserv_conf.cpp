#include "webserv_conf.hpp"
#include "route.hpp"

webserv_conf::webserv_conf(void)	{

	root = ".";
	index.push_back("index.html");
	http_version = "HTTP/1.1";
	server_name = "webserv (42) v0.1-dev";
	Route route1( "/", "./www" );
	Route route2( "/php", "./cgi" );
	routes.push_back( route1 );
	routes.push_back( route2 );
	routes.back().enable_cgi( "/usr/bin/php" );
	routes.at(0).add_error_page( 404, "defaultPages/404.html");
	routes.at(0).add_redirection( "moved.html", "/sub" );
}

webserv_conf::webserv_conf( webserv_conf &rhs ) : root(rhs.root) {};