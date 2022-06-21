#pragma once

#include "server.hpp"

class Bundle_server
{
	private:
		std::vector<Server_conf>	servers;
	public:
		Bundle_server();
		Bundle_server(Server_conf server);
		Bundle_server(std::vector<Server_conf> servers);

		void						 addServer(Server_conf server);
		std::vector<Server_conf>	 getServers() const;
		std::string					 getHost() const;
		unsigned short				 getPort() const;
		Server_conf					 get_server_from_server_name(std::string hostname);
		void						 print_servers(void);
		Bundle_server &  			 operator=( Bundle_server const & rhs );

};
