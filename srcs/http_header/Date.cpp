#include "class/request.hpp"
#include "class/response.hpp"
#include <iostream>
#include <string>
#include <ctime>

static inline std::string get_formatted_datetime()
{
	time_t rawtime;
	char buffer[80];

	time(&rawtime);

	strftime(buffer, 80, "%a, %d %b %G %T GMT", std::gmtime(&rawtime));

	return buffer;
}

int	http_header_date(Request &req, Response &res)
{
	(void)req;
	res.add_header("Date", get_formatted_datetime());
	return (1);
}
