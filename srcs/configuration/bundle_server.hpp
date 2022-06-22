#pragma once

#include "server.hpp"

class Bundle_server
{
	private:
		std::vector<Server_conf>	servers;
		std::string					_host;
		unsigned short				_port;
		Bundle_server &  			 operator=( Bundle_server const & rhs );

	public:
		Bundle_server();
		Bundle_server(Server_conf server, unsigned short port);
		Bundle_server(std::vector<Server_conf> servers, unsigned short port);

		void						 addServer(Server_conf server, unsigned short port);
		std::vector<Server_conf>	 getServers() const;
		std::string					 getHost() const;
		unsigned short				 getPort() const;
		Server_conf					 getMainServer() const;
		Server_conf					 get_server_from_server_name(std::string hostname);
		void						 print_servers(void);

};
