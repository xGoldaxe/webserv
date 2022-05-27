/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv_conf.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: datack <datack@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 16:05:17 by datack            #+#    #+#             */
/*   Updated: 2022/05/27 16:22:46 by datack           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv_conf.hpp"

// default values
Webserv_conf::Webserv_conf(void)
{
	http_version = "HTTP/1.1";
	Server_conf server = Server_conf();
	this->servers.push_back(server);
};

static int return_type_parse(std::string s)
{
	unsigned int i = 0;
	std::string tab[11] = {"server_name", "listen",
						   "error_page", "location",
						   "root", "index",
						   "methods", "enable_cgi",
						   "cgi_extension", "body_max_size", "server"};
	while (i < 11)
	{
		if (s.compare(tab[i]) == 0)
			return i;
		i++;
	}
	return (-1);
}

Webserv_conf::Webserv_conf(std::string filename)
{
	http_version = "HTTP/1.1";
	Server_conf server = Server_conf(1);

	std::ifstream file;
	std::string buffer;
	std::stringstream s;
	std::vector<std::string> words;
	std::string tmperrorval;
	size_t pos = 0;
	int check = -1;
	unsigned int it = 0;
	unsigned int tmpit = 0;
	int firstservswitch = 1;
	int contextlocation = 0;

	file.open(filename.c_str(), std::ifstream::in);
	if (!file.is_open())
	{
		throw Webserv_conf::FailedToOpenFile();
	}
	s << file.rdbuf();
	buffer = s.str();
	file.close();

	// buffer.erase(std::remove(buffer.begin(), buffer.end(), '\t'), buffer.end());
	std::replace(buffer.begin(), buffer.end(), '\t', ' ');
	std::replace(buffer.begin(), buffer.end(), '{', ' ');
	std::replace(buffer.begin(), buffer.end(), '}', ' ');
	std::replace(buffer.begin(), buffer.end(), '\n', ' ');

	while ((pos = buffer.find(" ")) != std::string::npos)
	{
		words.push_back(buffer.substr(0, pos));
		buffer.erase(0, pos + 1);
	}
	words.erase(std::remove(words.begin(), words.end(), ""), words.end());
	while (it < words.size())
	{
		check = return_type_parse(words[it]);
		std::cout << words[it] << "|"
				  << check << std::endl;
		switch (check)
		{
		case SERVER_NAME:
			if ((it + 3) < words.size() && words[it + 1].compare("=") == 0 && words[it + 3].compare(";") == 0)
			{
				server.setName(words[it + 2]);
				it = it + 3;
			}
			else
			{
				throw Webserv_conf::SussyParsing();
			}
			break;
		case LISTEN:
			if ((it + 3) < words.size() && words[it + 1].compare("=") == 0 && words[it + 3].compare(";") == 0)
			{
				int port = std::atoi(words[it + 2].c_str());
				std::numeric_limits<short> range;
				if (port < range.min() || port > range.max())
					throw Webserv_conf::OutOfRangePort();
				server.addPort(port);
				it = it + 3;
			}
			else
			{
				throw Webserv_conf::SussyParsing();
			}
			break;
		case ERROR_PAGE:
			// assumes syntax error_page a b ... z = "blablabla" ;
			//  get value after =
			tmpit = it;
			while (it < words.size() && words[it].compare("=") != 0)
				it++;
			if (words[it].compare("=") == 0 && it + 2 < words.size() && words[it + 2].compare(";") == 0 && ((it - tmpit) > 1))
			{
				tmperrorval = words[it + 1];
				tmpit++;
				while (tmpit < it)
				{
					server.addErrorPages(std::atoi(words[tmpit].c_str()), tmperrorval);
					tmpit++;
				}
				it = it + 2;
			}
			else
			{
				throw Webserv_conf::SussyParsing();
			}
			break;
		case LOCATION:
			contextlocation = 1;
			break;
		case ROOT:
			break;
		case INDEX:
			break;
		case METHODS:
			if(!contextlocation)
				throw Webserv_conf::SussyParsing();
			if ((it + 3) < words.size() && words[it + 1].compare("=") == 0)
			{
				it = it + 2;
				while (it < words.size() && words[it].compare(";") != 0)
				{
					server.addMethods(words[it]);
					it++;
				}
			}
			break;
		case ENABLE_CGI:
			break;
		case CGI_EXTENSION:
			break;
		case BODY_MAX_SIZE:
			if ((it + 3) < words.size() && words[it + 1].compare("=") == 0 && words[it + 3].compare(";") == 0)
			{
				server.setBodyMaxSize(std::atoi(words[it+2].c_str()));
				it = it + 3;
			}
			else
			{
				throw Webserv_conf::SussyParsing();
			}
			break;
		case SERVER:
			if (firstservswitch)
			{
				firstservswitch = 0;
				break;
			}
			contextlocation = 0;
			this->servers.push_back(server);
			server = Server_conf(1);
			break;
		default:
			break;
		}
		it++;
	}
	this->servers.push_back(server);
};
/*
void Webserv_conf::print_conf(void)
{
	std::cout << "root = " << this->root << std::endl;
	std::cout << "http version = " << this->http_version << std::endl;
	std::cout << "server_name = " << this->server_name << std::endl;

	std::cout << "index = " << std::endl;
	for (std::vector<std::string>::const_iterator i = (this->index).begin(); i != (this->index).end(); ++i)
		std::cout << *i << ' ';
	std::cout << std::endl;

	// for (std::vector<Route>::const_iterator i = (this->index).begin(); i != path.end(); ++i)
	//    std::cout << *i << ' ';
	std::cout << "ports = " << std::endl;
	for (std::list<int>::const_iterator k = (this->port).begin(); k != (this->port).end(); ++k)
		std::cout << *k << ' ';
	std::cout << std::endl;
}*/