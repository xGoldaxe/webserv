#include "webserv_conf.hpp"
#include "route.hpp"
#include <iostream>

webserv_conf::webserv_conf(void)	{

	root = ".";
	index.push_back("index.html");
	http_version = "HTTP/1.1";
	server_name = "webserv (42) v0.1-dev";
	Route route1( "/", "/mnt/nfs/homes/pleveque/Documents/webserv/www" );
	Route route2( "/php", "/mnt/nfs/homes/pleveque/Documents/webserv/cgi" );

	route1.add_error_page( 404, "defaultPages/404.html");
	route1.add_error_page( 500, "defaultPages/404.html");
	route1.add_redirection( "/moved.html", "/sub/index.html" );

	routes.push_back( route1 );
	routes.push_back( route2 );
	routes.back().enable_cgi( "/usr/bin/php-cgi" );
}

webserv_conf::webserv_conf( webserv_conf &rhs ) : root(rhs.root) {};