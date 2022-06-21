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
		Server_conf					 get_server_from_hostname(std::string hostname);
		void						 print_servers(void);
		Bundle_server &  			 operator=( Bundle_server const & rhs );

};
