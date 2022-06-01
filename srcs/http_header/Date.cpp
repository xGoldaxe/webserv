#include "../webserv.hpp"
#include <ctime>

static inline std::string get_formatted_datetime()
{
	time_t rawtime;
	struct tm *timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	// Wed, 21 Oct 2015 07:28:00 GMT
	strftime(buffer, 80, "%a, %d %b %G %T GMT", timeinfo);

	return buffer;
}

int	http_header_date(Response &res)
{
	res.add_header("Date", get_formatted_datetime());
	return (1);
}
