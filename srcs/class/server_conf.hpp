/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_conf.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: datack <datack@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 14:47:11 by datack            #+#    #+#             */
/*   Updated: 2022/05/26 15:23:50 by datack           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../webserv.hpp"
#include "webserv_conf.hpp"

class Server_conf
{

	private:
		std::list<int>				port;
		std::vector<Route>			routes;
		std::string					server_name = "webserv (42) v0.1-dev";
		std::vector<std::string>	index;
	public:
		Server_conf(void);
		
		std::list<int>				getPort();
		std::vector<Route>			getRoutes();
		std::string					getName();
		std::vector<std::string>	getIndex();
		void						addPort(int port);
		void 						addRoute(Route route);
		void 						setName(std::string server_name);
		void						addIndex(std::string index);
};