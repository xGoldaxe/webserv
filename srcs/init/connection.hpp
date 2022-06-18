#pragma once

#include <string>
#include <ctime>
#include <queue>
#include "../class/route.hpp"
#include "../class/request.hpp"
#include "../class/response.hpp"

class Request;
class Response;

class Connection
{

	private:
		int						_fd;
		Request					*_req;
		std::string				_raw_data;
		bool					_is_init;
		time_t	 				_begin_time;
		std::queue<Request *>	_requests;
		bool					_is_sending_data;
		bool					_is_dead;
		const char				*_client_ip;
		size_t					_response_max_size;
		bool					_is_new_data;

		Connection( void );
		Connection &   operator=( Connection const & rhs );

		bool					_data_added();

	public:

		/* coplien */
		Connection( int fd, char *client_ip, size_t response_chunk_size );
		Connection( Connection const &src );
		~Connection( void );

		/* end coplien */
		bool	is_ready();
		bool	is_fulfilled();
		bool	is_invalid_req();
		void	read_data();
		void	queue_iteration(std::vector<Route> routes);
		bool	check_state();
		void	process();
		void	soft_clear();
		bool	init_request(std::vector<Route> routes);
		Request	*extract_request();
		void	end_send();
		/* error case */
		bool	is_timeout(void);

		/* getter */
		int			get_fd() const;
		Request 	*get_req() const;
		std::string	get_data() const;
		bool		is_init() const;
		time_t		get_time() const;
		bool		get_is_dead(void) const;
		bool		get_is_sending_data(void) const;
		std::string get_client_ip(void) const;
		bool 		get_is_new_data(void) const;
};
