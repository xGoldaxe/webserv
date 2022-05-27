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

#ifndef SERVER_CONF_HPP
# define SERVER_CONF_HPP

#include "route.hpp"
#include <list>

class Route;

class Server_conf
{

	private:
		std::list<short>			port;
		std::vector<Route>			routes;
		std::string					server_name;
		std::vector<std::string>	index;
		int							body_max_size;
		std::string 				root;
	public:
		Server_conf(void);
		Server_conf(int emp);

		std::list<short>				getPort() const;
		std::vector<Route>				getRoutes() const;
		std::string						getName() const;
		std::vector<std::string>		getIndex() const;
		int								getBodyMaxSize() const;
		std::string						getRoot() const;
		void							addPort(short port);
		void 							addRoute(Route route);
		void 							setName(std::string server_name);
		void							addIndex(std::string index);
		void							addIndexRoute(std::string index);
		void 							addMethods(std::string methods);
		void 							addErrorPages(int error, std::string errorpage);
		void							setBodyMaxSize(int body_max_size);
		void							setRoot(std::string root);


};
#endif