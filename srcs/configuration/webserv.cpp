#include "webserv.hpp"

Webserv_conf &Webserv_conf::operator=(Webserv_conf const &rhs)
{
	this->servers = rhs.getServers();
	this->http_version = rhs.getHttpVersion();
	return *this;
}

// default values
Webserv_conf::Webserv_conf(void)
{
	http_version = "HTTP/1.1";
	Server_conf server = Server_conf();
	this->servers.push_back(server);
}

Webserv_conf::~Webserv_conf(void)
{
}


static int return_type_parse(std::string s)
{
	unsigned int i = 0;
	std::string tab1[SIZE_PARSING] = {"server_name", "listen",
									  "error_page", "location",
									  "root", "index",
									  "methods", "enable_cgi",
									  "cgi_extension", "body_max_size", "server",
									  "rewrite", "autoindex", "cgi_timeout", "read_timeout",
									  "server_body_size", "send_file", "file_limit", "client_header_size",
									  "host", "max_amount_of_request", "max_uri_size", "cgi_path",
									  "run_file_path", "chunk_head_limit", "chunk_body_limit"};
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

static bool verify_run_file_path(std::string url)
{
	if (!(url.at(url.size() - 1) == '/'))
		return (false);
	std::string tested_url;
	for (std::string::iterator it = url.begin(); it != url.end(); ++it)
	{
		if (*it == '?' || *it == ',')
			break;
		tested_url += *it;
	}

	int nb_sub = 0;
	int nb_regular = 0;
	int nb_back = 0;
	std::string actual;
	for (std::string::iterator it = tested_url.begin(); it != tested_url.end(); ++it)
	{
		if (*it == '/' && actual.size() > 0) /* we ignore ////qwe///qwe//ewq case */
		{
			// std::cout << actual << std::endl;
			actual != ".." ? ++nb_regular : ++nb_back;
			++nb_sub;
			actual = "";
		}
		else if (*it != '/')
			actual += *it;
	}
	if (actual.size() != 0) // case where there is no / at end
	{
		// std::cout << actual << std::endl;
		actual != ".." ? ++nb_regular : ++nb_back;
		++nb_sub;
	}

	// std::cout << nb_sub << " " << nb_regular << " " << nb_back << std::endl;
	if (nb_sub == 0 && !(tested_url.size() > 0 && tested_url[0] == '/'))
		return false;
	if (tested_url.size() == 0 || tested_url[0] != '/')
		return false;
	if (nb_back > nb_regular)
		return false;

	return true;
}

static bool verify_url(std::string url)
{
	if (url.find("http://") == 0 || url.find("https://") == 0)
		return true;
	std::string tested_url;
	for (std::string::iterator it = url.begin(); it != url.end(); ++it)
	{
		if (*it == '?' || *it == ',')
			break;
		tested_url += *it;
	}

	int nb_sub = 0;
	int nb_regular = 0;
	int nb_back = 0;
	std::string actual;
	for (std::string::iterator it = tested_url.begin(); it != tested_url.end(); ++it)
	{
		if (*it == '/' && actual.size() > 0) /* we ignore ////qwe///qwe//ewq case */
		{
			// std::cout << actual << std::endl;
			actual != ".." ? ++nb_regular : ++nb_back;
			++nb_sub;
			actual = "";
		}
		else if (*it != '/')
			actual += *it;
	}
	if (actual.size() != 0) // case where there is no / at end
	{
		// std::cout << actual << std::endl;
		actual != ".." ? ++nb_regular : ++nb_back;
		++nb_sub;
	}

	// std::cout << nb_sub << " " << nb_regular << " " << nb_back << std::endl;
	if (nb_sub == 0 && !(tested_url.size() > 0 && tested_url[0] == '/'))
		return false;
	if (tested_url.size() == 0 || tested_url[0] != '/')
		return false;
	if (nb_back > nb_regular)
		return false;

	return true;
}

std::vector<Server_conf> Webserv_conf::getServers() const
{
	return this->servers;
}

std::vector<Bundle_server> Webserv_conf::getBundleServers() const
{
	return this->bundle_servers;
}

std::string Webserv_conf::getHttpVersion() const
{
	return this->http_version;
}

static int check_if_config_is_proper(std::string buffer)
{
	std::vector<std::string> words;
	size_t pos;
	std::string copy;
	unsigned int it = 0;
	bool insideserver = false;
	bool insidelocation = false;
	bool bracketserver = false;
	bool bracketlocation = false;

	copy.append(buffer);

	copy.append("\n");

	std::replace(copy.begin(), copy.end(), '\t', ' ');
	std::replace(copy.begin(), copy.end(), '\n', ' ');

	while ((pos = copy.find(" ")) != std::string::npos)
	{
		words.push_back(copy.substr(0, pos));
		copy.erase(0, pos + 1);
	}

	while (it < words.size())
	{
		if (words[it].compare("server") == 0)
		{
			if (insideserver || insidelocation || bracketserver || bracketlocation)
				return (-1);
			insideserver = true;
		}
		if (words[it].compare("location") == 0)
		{
			if ((insideserver && !bracketserver) || insidelocation || !bracketserver || bracketlocation)
				return (-1);
			insidelocation = true;
		}
		if (words[it].compare("{") == 0)
		{
			if ((insideserver && insidelocation && !bracketserver) || (bracketserver && insideserver && !insidelocation) || bracketlocation || (!insideserver && !insidelocation))
				return (-1);
			if (!insidelocation)
				bracketserver = true;
			if (insidelocation)
				bracketlocation = true;
		}
		if (words[it].compare("}") == 0)
		{
			if ((!insideserver && !insidelocation) || (!insideserver && !bracketserver) || (insidelocation && !bracketlocation))
				return (-1);
			if (insidelocation)
			{
				bracketlocation = false;
				insidelocation = false;
			}
			else
			{
				if (insideserver)
				{
					bracketserver = false;
					insideserver = false;
				}
			}
		}
		it++;
	}

	if (insideserver || insidelocation || bracketserver || bracketlocation)
		return (-1);
	return (1);
}

static std::string parsing_open_file(std::string filename)
{
	std::ifstream file;
	std::stringstream buffer;
	file.open(filename.c_str(), std::ifstream::in);
	if (!file.is_open())
	{
		throw Webserv_conf::FailedToOpenFile();
	}
	buffer << file.rdbuf();
	file.close();
	return buffer.str();
}

static void	check_server_dupe(std::vector<Server_conf> servers)
{
	unsigned int i_checkservdupe = 0;
	unsigned int j_checkservdupe = 1;
	unsigned int i_checkportdupe = 0;
	unsigned int j_checkportdupe = 0;
	while (i_checkservdupe < servers.size())
	{
		while (j_checkservdupe < servers.size())
		{
			if (servers[i_checkservdupe].getHost().compare(servers[j_checkservdupe].getHost()) == 0
			|| (servers[i_checkservdupe].getHost().compare("0.0.0.0") == 0)
			|| (servers[j_checkservdupe].getHost().compare("0.0.0.0") == 0 ))
			{
				while (i_checkportdupe < servers[i_checkservdupe].getPort().size())
				{
					while (j_checkportdupe < servers[j_checkservdupe].getPort().size())
					{
						// if (servers[i_checkservdupe].getPort()[i_checkportdupe] == servers[j_checkservdupe].getPort()[j_checkportdupe])
						// 	throw std::invalid_argument("Parsing error, duplicate port detected!");
						j_checkportdupe++;
					}
					j_checkportdupe = 0;
					i_checkportdupe++;
				}
				i_checkportdupe = 0;
			}
			j_checkservdupe++;
		}
		i_checkservdupe++;
		j_checkservdupe = i_checkservdupe + 1;
	}
}

Webserv_conf::Webserv_conf(std::string filename)
{
	/**
	 * @TODO Besoin d'une refracto de cette fonction! Séparation des différentes tâches.
	 */

	http_version = "HTTP/1.1";
	Server_conf server = Server_conf(1);

	std::string buffer;
	std::vector<std::string> words;
	std::string tmperrorval;
	size_t pos = 0;
	int check = -1;
	unsigned int it = 0;
	unsigned int tmpit = 0;
	int firstservswitch = 1;
	int contextlocation = 0;
	int error;

	buffer = parsing_open_file(filename);

	if (check_if_config_is_proper(buffer) == -1)
		throw std::invalid_argument("The configuration file is invalid!");

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
		throw std::invalid_argument("Configuration file does not start with 'server'");
	while (it < words.size())
	{
		check = return_type_parse(words[it]);
		switch (check)
		{
		case SERVER_NAME_PARSING:
			if (firstservswitch)
				throw std::invalid_argument("Error parsing, no server was defined");
			if (contextlocation)
				throw std::invalid_argument("Error parsing, encountered server_name in a location");
			if ((it + 3) < words.size() && words[it + 1].compare("=") == 0)
			{
				it = it + 2;
				while (it < words.size() && words[it].compare(";") != 0)
				{
					server.setName(words[it]);
					it++;
				}
			}
			else
			{
				throw std::invalid_argument("Error parsing Index!");
			}
			break;
		case LISTEN_PARSING:
			if (firstservswitch)
				throw std::invalid_argument("Error parsing, no server was defined");
			if (contextlocation)
				throw std::invalid_argument("Error parsing, encountered port in a location");
			if ((it + 3) < words.size() && words[it + 1].compare("=") == 0 && words[it + 3].compare(";") == 0)
			{
				int port = std::atoi(words[it + 2].c_str());
				std::numeric_limits<unsigned short> range;
				if (port < 1024 || port > range.max())
					throw Webserv_conf::OutOfRangePort();
				server.addPort((unsigned short)port);
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
			if (firstservswitch)
				throw std::invalid_argument("Error parsing, no server was defined");
			tmpit = it;
			while (it < words.size() && words[it].compare("=") != 0)
				it++;
			if (!verify_url(words[it + 1]))
				throw std::invalid_argument("Invalid error page path");
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
			if (firstservswitch)
				throw std::invalid_argument("Error parsing, no server was defined");
			contextlocation = 1;
			if (!verify_url(words[it + 1]))
				throw std::invalid_argument("Invalid location path");
			if ((it + 1) < words.size())
			{
				server.addRoute(Route(words[it + 1], 1));
			}
			else
			{
				throw std::invalid_argument("Error parsing routes");
			}
			break;
		case ROOT_PARSING:
			if (firstservswitch)
				throw std::invalid_argument("Error parsing, no server was defined");
			if ((it + 2) < words.size() && words[it + 2].compare(";") == 0)
			{
				if (!verify_run_file_path(words[it + 1]))
					throw std::invalid_argument("Invalid root path");
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
			if (firstservswitch)
				throw std::invalid_argument("Error parsing, no server was defined");
			if ((it + 3) < words.size() && words[it + 1].compare("=") == 0)
			{
				it = it + 2;
				while (it < words.size() && words[it].compare(";") != 0)
				{
					if (!verify_url(words[it]))
						throw std::invalid_argument("Invalid index path");
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
			if (firstservswitch)
				throw std::invalid_argument("Error parsing, no server was defined");
			if (!contextlocation)
				throw std::invalid_argument("Error parsing, encountered methods outside of a location");
			if ((it + 3) < words.size() && words[it + 1].compare("=") == 0)
			{
				it = it + 2;
				while (it < words.size() && words[it].compare(";") != 0)
				{
					if (words[it].compare("GET") != 0 && words[it].compare("POST") != 0 && words[it].compare("DELETE") != 0 && words[it].compare("HEAD") != 0 && words[it].compare("PUT") != 0)
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
			// location bool
			if (firstservswitch)
				throw std::invalid_argument("Error parsing, no server was defined");
			if (!contextlocation)
				throw std::invalid_argument("Error parsing, encountered enable cgi outside of location");
			if ((it + 2) < words.size() && words[it + 2].compare(";") == 0 && (words[it + 1].compare("on") == 0 || words[it + 1].compare("off") == 0))
			{
				if (words[it + 1].compare("on") == 0)
					server.set_enable_cgi(true);
				else
					server.set_enable_cgi(false);
				it = it + 2;
			}
			else
			{
				throw std::invalid_argument("Error parsing enable_cgi");
			}
			break;
		case CGI_EXTENSION_PARSING:
			// location cgi_extension machin machin ... ;
			if (firstservswitch)
				throw std::invalid_argument("Error parsing, no server was defined");
			if (!contextlocation)
				throw std::invalid_argument("Error parsing, encountered cgi extension outside of location");
			if ((it + 3) < words.size() && words[it + 1].compare("=") == 0)
			{
				it = it + 2;
				while (it < words.size() && words[it].compare(";") != 0)
				{
					server.addRouteCGIExtension(words[it]);
					it++;
				}
			}
			else
			{
				throw std::invalid_argument("Error parsing Methods!");
			}
			break;
		case BODY_MAX_SIZE_PARSING:
			// server location int
			if (firstservswitch)
				throw std::invalid_argument("Error parsing, no server was defined");

			if ((it + 3) < words.size() && words[it + 1].compare("=") == 0 && words[it + 3].compare(";") == 0)
			{
				if (std::atoi(words[it + 2].c_str()) < 1)
					throw std::invalid_argument("Error parsing body_max_size, must be 1 or greater");
				if (contextlocation)
					server.setBodyMaxSizeRoute(std::atoi(words[it + 2].c_str()));
				else
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
			// location
			// rewrite int url url ;
			if (firstservswitch)
				throw std::invalid_argument("Error parsing, no server was defined");
			if (!contextlocation)
				throw std::invalid_argument("Error parsing, encountered redirection outside of location");
			if ((it + 4) < words.size() && words[it + 4].compare(";") == 0)
			{

				if (std::atoi(words[it + 1].c_str()) > 399 || std::atoi(words[it + 1].c_str()) < 300)
					throw std::invalid_argument("Error parsing, provided redirection code is outside the range");
				if (!verify_url(words[it + 2]) || !verify_url(words[it + 3]))
					throw std::invalid_argument("Invalid redirection path");
				server.addRouteRedirection(std::atoi(words[it + 1].c_str()), words[it + 2], words[it + 3]);
				it = it + 4;
			}
			else
			{
				throw std::invalid_argument("Error parsing rewrite!");
			}
			break;
		case AUTOINDEX_PARSING:
			// location bool
			if (firstservswitch)
				throw std::invalid_argument("Error parsing, no server was defined");
			if (!contextlocation)
				throw std::invalid_argument("Error parsing, encountered auto index outside of location");
			if ((it + 2) < words.size() && words[it + 2].compare(";") == 0 && (words[it + 1].compare("on") == 0 || words[it + 1].compare("off") == 0))
			{
				if (words[it + 1].compare("on") == 0)
					server.setRouteAutoIndex(true);
				else
					server.setRouteAutoIndex(false);
				it = it + 2;
			}
			else
			{
				throw std::invalid_argument("Error parsing autoindex");
			}
			break;
		case CGI_TIMEOUT_PARSING:
			// location int seconds
			if (firstservswitch)
				throw std::invalid_argument("Error parsing, no server was defined");
			if (!contextlocation)
				throw std::invalid_argument("Error parsing, encountered cgi timeout outside of location");
			if ((it + 3) < words.size() && words[it + 1].compare("=") == 0 && words[it + 3].compare(";") == 0)
			{
				if (std::atoi(words[it + 2].c_str()) < 1)
					throw std::invalid_argument("Error parsing cgi_timeout, must be 1 or greater");
				server.set_cgi_timeout(std::atoi(words[it + 2].c_str()));
				it = it + 3;
			}
			else
			{
				throw std::invalid_argument("Error parsing CGI Timeout!");
			}
			break;
		case READ_TIMEOUT_PARSING:
			// server int seconds
			if (firstservswitch)
				throw std::invalid_argument("Error parsing, no server was defined");
			if (contextlocation)
				throw std::invalid_argument("Error parsing, encountered read timeout in a location");
			if ((it + 3) < words.size() && words[it + 1].compare("=") == 0 && words[it + 3].compare(";") == 0)
			{
				if (std::atoi(words[it + 2].c_str()) < 1)
					throw std::invalid_argument("Error parsing read_timeout, must be 1 or greater");
				server.setReadTimeOut(std::atoi(words[it + 2].c_str()));
				it = it + 3;
			}
			else
			{
				throw std::invalid_argument("Error parsing Read Timeout!");
			}
			break;
		case SERVER_BODY_SIZE_PARSING:
			// server int
			if (firstservswitch)
				throw std::invalid_argument("Error parsing, no server was defined");
			if (contextlocation)
				throw std::invalid_argument("Error parsing, encountered server body size in a location");
			if ((it + 3) < words.size() && words[it + 1].compare("=") == 0 && words[it + 3].compare(";") == 0)
			{
				if (std::atoi(words[it + 2].c_str()) < 1)
					throw std::invalid_argument("Error parsing read_timeout, must be 1 or greater");
				server.setServerBodySize(std::atoi(words[it + 2].c_str()));
				it = it + 3;
			}
			else
			{
				throw std::invalid_argument("Error parsing server_body_size!");
			}
			break;
		case SEND_FILE_PARSING:
			// location bool
			if (firstservswitch)
				throw std::invalid_argument("Error parsing, no server was defined");
			if (!contextlocation)
				throw std::invalid_argument("Error parsing, encountered send_file outside of location");
			if ((it + 2) < words.size() && words[it + 2].compare(";") == 0 && (words[it + 1].compare("on") == 0 || words[it + 1].compare("off") == 0))
			{
				if (words[it + 1].compare("on") == 0)
					server.set_send_file(true);
				else
					server.set_send_file(false);
				it = it + 2;
			}
			else
			{
				throw std::invalid_argument("Error parsing send_file");
			}
			break;
		case FILE_LIMIT_PARSING:
			// location int megabytes
			if (firstservswitch)
				throw std::invalid_argument("Error parsing, no server was defined");
			if (!contextlocation)
				throw std::invalid_argument("Error parsing, encountered file_limit outside of location");
			if ((it + 3) < words.size() && words[it + 1].compare("=") == 0 && words[it + 3].compare(";") == 0)
			{
				if (std::atoi(words[it + 2].c_str()) < 1)
					throw std::invalid_argument("Error parsing file_limit_parsing, must be 1 or greater");
				server.set_file_limit(std::atoi(words[it + 2].c_str()));
				it = it + 3;
			}
			else
			{
				throw std::invalid_argument("Error parsing file_limit!");
			}
			break;
		case CLIENT_HEADER_SIZE_PARSING:
			// server int
			if (firstservswitch)
				throw std::invalid_argument("Error parsing, no server was defined");
			if (contextlocation)
				throw std::invalid_argument("Error parsing, encountered client_header_size in a location");
			if ((it + 3) < words.size() && words[it + 1].compare("=") == 0 && words[it + 3].compare(";") == 0)
			{
				if (std::atoi(words[it + 2].c_str()) < 1)
					throw std::invalid_argument("Error parsing client_header_size, must be 1 or greater");
				server.setClientHeaderSize(std::atoi(words[it + 2].c_str()));
				it = it + 3;
			}
			else
			{
				throw std::invalid_argument("Error parsing client_header_size!");
			}
			break;
		case HOST_PARSING:
			// server string
			if (firstservswitch)
				throw std::invalid_argument("Error parsing, no server was defined");
			if (contextlocation)
				throw std::invalid_argument("Error parsing, encountered host in a location");
			if ((it + 3) < words.size() && words[it + 1].compare("=") == 0 && words[it + 3].compare(";") == 0)
			{
				server.setHost(words[it + 2]);
				it = it + 3;
			}
			else
			{
				throw std::invalid_argument("Error parsing host");
			}
			break;
		case MAX_AMOUNT_OF_REQUEST_PARSING:
			// server int
			if (firstservswitch)
				throw std::invalid_argument("Error parsing, no server was defined");
			if (contextlocation)
				throw std::invalid_argument("Error parsing, encountered max_amount_of_request in a location");
			if ((it + 3) < words.size() && words[it + 1].compare("=") == 0 && words[it + 3].compare(";") == 0)
			{
				if (std::atoi(words[it + 2].c_str()) < 1)
					throw std::invalid_argument("Error parsing, the provided max_amount_of_request is inferior to 1!");
				server.set_max_amount_of_request(std::atoi(words[it + 2].c_str()));
				it = it + 3;
			}
			else
			{
				throw std::invalid_argument("Error parsing max_amount_of_request");
			}
			break;
		case MAX_URI_SIZE_PARSING:
			// server int
			if (firstservswitch)
				throw std::invalid_argument("Error parsing, no server was defined");
			if (contextlocation)
				throw std::invalid_argument("Error parsing, encountered max_amount_of_request in a location");
			if ((it + 3) < words.size() && words[it + 1].compare("=") == 0 && words[it + 3].compare(";") == 0)
			{
				if (std::atoi(words[it + 2].c_str()) < 30)
					throw std::invalid_argument("Error parsing, the provided max_uri_size is inferior to 30!");
				server.set_max_uri_size(std::atoi(words[it + 2].c_str()));
				it = it + 3;
			}
			else
			{
				throw std::invalid_argument("Error parsing max_uri_size");
			}
			break;
		case CGI_PATH_PARSING:
			if (firstservswitch)
				throw std::invalid_argument("Error parsing, no server was defined");
			if (!contextlocation)
				throw std::invalid_argument("Error parsing, encountered cgi_path outside of location");
			if ((it + 3) < words.size() && words[it + 1].compare("=") == 0 && words[it + 3].compare(";") == 0)
			{
				if (!verify_url(words[it + 2]))
					throw std::invalid_argument("Invalid cgi path");
				server.set_cgi_path(words[it + 2]);
				it = it + 3;
			}
			else
			{
				throw std::invalid_argument("Error parsing cgi_path!");
			}
			break;
		case RUN_FILE_PATH_PARSING:
			if (firstservswitch)
				throw std::invalid_argument("Error parsing, no server was defined");
			if (contextlocation)
				throw std::invalid_argument("Error parsing, encountered run_file_path outside of server");
			if ((it + 3) < words.size() && words[it + 1].compare("=") == 0 && words[it + 3].compare(";") == 0)
			{
				if (!verify_run_file_path(words[it + 2]))
					throw std::invalid_argument("Invalid run file path!");
				server.setRunFilePath(words[it + 2]);
				it = it + 3;
			}
			else
			{
				throw std::invalid_argument("Error parsing run_file_path!");
			}
			break;
		case CHUNK_HEAD_LIMIT_PARSING:
			if (firstservswitch)
				throw std::invalid_argument("Error parsing, no server was defined");
			if ((it + 3) < words.size() && words[it + 1].compare("=") == 0 && words[it + 3].compare(";") == 0)
			{
				if (std::atoi(words[it + 2].c_str()) < 1)
					throw std::invalid_argument("Error parsing Chunk Head Limit, must be 1 or greater");
				if (contextlocation)
					server.setChunkHeadLimitRoute(std::atoi(words[it + 2].c_str()));
				else
					server.setChunkHeadLimit(std::atoi(words[it + 2].c_str()));

				it = it + 3;
			}
			else
			{
				throw std::invalid_argument("Error parsing Chunk Head Limit!");
			}
			break;
		case CHUNK_BODY_LIMIT_PARSING:
			if (firstservswitch)
				throw std::invalid_argument("Error parsing, no server was defined");

			if ((it + 3) < words.size() && words[it + 1].compare("=") == 0 && words[it + 3].compare(";") == 0)
			{
				if (std::atoi(words[it + 2].c_str()) < 1)
					throw std::invalid_argument("Error parsing Chunk Body Limit, must be 1 or greater");
				if (contextlocation)
					server.setChunkBodyLimitRoute(std::atoi(words[it + 2].c_str()));
				else
					server.setChunkBodyLimit(std::atoi(words[it + 2].c_str()));

				it = it + 3;
			}
			else
			{
				throw std::invalid_argument("Error parsing Chunk Body Limit!");
			}
			break;
		default:
			break;
		}
		it++;
	}
	this->servers.push_back(server);

	// Assign default variables to servers if none defined in parsing
	unsigned int checkservers = 0;
	while (checkservers < this->servers.size())
	{
		if (this->servers[checkservers].getName().empty())
			this->servers[checkservers].setName(DEFAULT_SERVER_NAME);

		if (this->servers[checkservers].getRoutes().empty())
			throw std::invalid_argument("A server has no location!");

		if (this->servers[checkservers].getPort().empty())
			this->servers[checkservers].addPort(DEFAULT_PORT);

		this->servers[checkservers].check_methods_route();
		checkservers++;
	}

	// Duplicate check
	check_server_dupe(this->servers);

/*

	std::vector<std::vector<Server_conf> > sorted_servers = std::vector<std::vector<Server_conf> >();

	// iterate over servers and add them to the vector of servers
	unsigned int jterator = 0;
	for (unsigned int i = 0; i < this->servers.size(); i++)
	{
		while (jterator < sorted_servers.size())
		{
			if (!sorted_servers[jterator].empty()
				&& sorted_servers[jterator][0].getHost() == servers[i].getHost()
				&& sorted_servers[jterator][0].getPort() == servers[i].getPort())
			{
				sorted_servers[jterator].push_back(this->servers[i]);
				break;
			}
			jterator++;
		}
		if (jterator == sorted_servers.size())
		{
			sorted_servers.push_back(std::vector<Server_conf>());
			sorted_servers.back().push_back(this->servers[i]);
		}
		
		jterator = 0;
	}
*/

/*
	// bundle all the results into the bundled_servers vector
	for (unsigned int i = 0; i < sorted_servers.size(); i++)
	{
		this->bundle_servers.push_back(Bundle_server());
		for (unsigned int j = 0; j < sorted_servers[i].size(); j++)
		{
			this->bundle_servers[i].addServer(sorted_servers[i][j]);
		}
	}
*/
	unsigned int jterator = 0;
	for (unsigned int i = 0; i < this->servers.size(); i++)
	{
		while (jterator < this->bundle_servers.size())
		{
			if (!this->bundle_servers[jterator].getServers().empty()
				&& this->bundle_servers[jterator].getServers()[0].getHost() == servers[i].getHost()
				&& this->bundle_servers[jterator].getServers()[0].getPort() == servers[i].getPort())
			{
				this->bundle_servers[jterator].addServer(this->servers[i]);
				break;
			}
			jterator++;
		}
		if (jterator == this->bundle_servers.size())
		{
			this->bundle_servers.push_back(Bundle_server(this->servers[i]));
		}
		
		jterator = 0;
	}


	print_bundled_servers();
	std::cout << "dogen" << std::endl;
}

void Webserv_conf::print_bundled_servers()
{
	std::cout << "hello";
	for (unsigned int i = 0; i < this->bundle_servers.size(); i++)
	{
		std::cout << "************Vector " << i << " *************" << std::endl;
		this->bundle_servers[i].print_servers();
	}
}

