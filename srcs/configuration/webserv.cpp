#include "webserv.hpp"

// default values
Webserv_conf::Webserv_conf(void)
{
	http_version = "HTTP/1.1";
	Server_conf server = Server_conf();
	this->servers.push_back(server);
}

static int return_type_parse(std::string s)
{
	unsigned int i = 0;
	std::string tab1[SIZE_PARSING] = {"server_name", "listen",
									  "error_page", "location",
									  "root", "index",
									  "methods", "enable_cgi",
									  "cgi_extension", "body_max_size", "server",
									  "rewrite", "autoindex"};
	// initializing vector like an array is only available at CPP 11+
	// forced to create a regular array before putting inside a vector
	std::vector<std::string> tab(&tab1[0], &tab1[SIZE_PARSING]);

	while (i < SIZE_PARSING)
	{
		if (s.compare(tab[i]) == 0)
			return i;
		i++;
	}
	return (-1);
}

std::vector<Server_conf> Webserv_conf::getServers() const
{
	return this->servers;
}

std::string Webserv_conf::getHttpVersion() const
{
	return this->http_version;
}

Webserv_conf::Webserv_conf(std::string filename)
{
	/**
	 * @TODO Besoin d'une refracto de cette fonction! Séparation des différentes tâches.
	 */

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
	int error;

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

	if (words.empty())
		throw std::invalid_argument("Config file is empty");
	if (words.size() > 0 && words[0].compare("server") != 0)
		throw std::invalid_argument("Config file does not start with 'server'");
	while (it < words.size())
	{
		check = return_type_parse(words[it]);
		//		std::cout << words[it] << "|"
		//				  << check << std::endl;
		switch (check)
		{
		case SERVER_NAME_PARSING:
			if ((it + 3) < words.size() && words[it + 1].compare("=") == 0 && words[it + 3].compare(";") == 0)
			{
				server.setName(words[it + 2]);
				it = it + 3;
			}
			else
			{
				throw std::invalid_argument("Error parsing server_name");
			}
			break;
		case LISTEN_PARSING:
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
				throw std::invalid_argument("Error parsing listen");
			}
			break;
		case ERROR_PAGE_PARSING:
			// assumes syntax error_page a b ... z = "blablabla" ;
			//  get value after =
			tmpit = it;
			while (it < words.size() && words[it].compare("=") != 0)
				it++;
			if (words[it].compare("=") == 0 && it + 2 < words.size() && words[it + 2].compare(";") == 0 && ((it - tmpit) > 1))
			{
				tmperrorval = words[it + 1].c_str();
				tmpit++;
				while (tmpit < it)
				{
					error = std::atoi(words[tmpit].c_str());
					if (error < 400 || error > 599)
						throw std::invalid_argument("Error code provided in configuration file is outside the 400-599 range");
					if (contextlocation == 0)
						server.addErrorPages(error, tmperrorval);
					else
					{
						server.addLastRouteErrorPages(error, tmperrorval);
					}
					tmpit++;
				}
				it = it + 2;
			}
			else
			{
				throw std::invalid_argument("Error parsing error_pages");
			}
			break;
		case LOCATION_PARSING:
			contextlocation = 1;
			if ((it + 3) < words.size() && words[it + 2].compare("root") == 0)
			{
				server.addRoute(Route(words[it + 1], words[it + 3], 1));
			}
			else if (!((it + 1) < words.size()))
			{
				server.addRoute(Route(words[it + 1]));
			}
			else
			{
				throw std::invalid_argument("Error parsing routes");
			}
			break;
		case ROOT_PARSING:
			if ((it + 2) < words.size() && words[it + 2].compare(";") == 0)
			{
				if (contextlocation == 0)
					server.setName(words[it + 1]);
				else
					server.setRouteRoot(words[it + 1]);
				it = it + 2;
			}
			else
			{
				throw std::invalid_argument("Error parsing root");
			}
			break;
		case INDEX_PARSING:
			if ((it + 3) < words.size() && words[it + 1].compare("=") == 0)
			{
				it = it + 2;
				while (it < words.size() && words[it].compare(";") != 0)
				{
					if (contextlocation == 0)
						server.addIndex(words[it]);
					else
						server.addIndexRoute(words[it]);
					it++;
				}
			}
			else
			{
				throw std::invalid_argument("Error parsing Index!");
			}
			break;
		case METHODS_PARSING:
			if (!contextlocation)
				throw std::invalid_argument("Error parsing, encountered methods outside of a location");
			if ((it + 3) < words.size() && words[it + 1].compare("=") == 0)
			{
				it = it + 2;
				while (it < words.size() && words[it].compare(";") != 0)
				{
					if (words[it].compare("GET") != 0 && words[it].compare("POST") != 0 && words[it].compare("DELETE") != 0 && words[it].compare("HEAD") != 0)
					{
						throw std::invalid_argument("Error parsing Methods! This is invalid: " + words[it]);
					}
					server.addMethods(words[it]);
					it++;
				}
			}
			else
			{
				throw std::invalid_argument("Error parsing Methods!");
			}
			break;
		case ENABLE_CGI_PARSING:
			if (!contextlocation)
				throw std::invalid_argument("Error parsing, encountered enable cgi outside of location");
			break;
		case CGI_EXTENSION_PARSING:
			if (!contextlocation)
				throw std::invalid_argument("Error parsing, encountered cgi extension outside of location");
			break;
		case BODY_MAX_SIZE_PARSING:
			if ((it + 3) < words.size() && words[it + 1].compare("=") == 0 && words[it + 3].compare(";") == 0)
			{
				server.setBodyMaxSize(std::atoi(words[it + 2].c_str()));
				it = it + 3;
			}
			else
			{
				throw std::invalid_argument("Error parsing Body Max Size!");
			}
			break;
		case SERVER_PARSING:
			if (firstservswitch)
			{
				firstservswitch = 0;
				break;
			}
			contextlocation = 0;
			this->servers.push_back(server);
			server = Server_conf(1);
			break;
		case REWRITE_PARSING:
			break;
		case AUTOINDEX_PARSING:
			break;
		default:
			break;
		}
		it++;
	}
	this->servers.push_back(server);

	#ifdef DEBUG
	std::vector<Server_conf> vecdebug = this->servers;
	unsigned int iterdebug = 0;
	while (iterdebug < vecdebug.size())
	{
		vecdebug[iterdebug].printServer();
		iterdebug++;
	}
	#endif
}
