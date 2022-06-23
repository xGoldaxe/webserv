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
		size_t	 				_begin_time;
		std::queue<Request *>	_requests;
		bool					_is_sending_data;
		bool					_is_dead;
		const char				*_client_ip;
		bool					_is_new_data;
		/* from server conf */
		size_t					_response_max_size;
		size_t					_onread_timeout;
		size_t					_idle_timeout;
		size_t					_max_size;
		size_t					_max_request;
		size_t					_process_data_size;
		Server_conf				_conf;

		Connection( void );
		Connection &   operator=( Connection const & rhs );

		bool					_data_added();
		void					_clean_exit();

	public:

		/* coplien */
		Connection( int fd, char *client_ip, const Server_conf & response_chunk_size );
		Connection( Connection const &src );
		~Connection( void );

		/* end coplien */
		void	update_timeout();
		bool	is_ready();
		bool	is_fulfilled();
		bool	is_invalid_req();
		void	read_data();
		void	queue_iteration(Bundle_server bundle);
		bool	check_state();
		void	process();
		void	soft_clear();
		bool	init_request(Bundle_server bundle);
		Request	*extract_request();
		void	end_send();
		/* error case */
		bool	is_timeout(void);

		/* getter */
		int			get_fd() const;
		Request 	*get_req() const;
		std::string	get_data() const;
		bool		is_init() const;
		size_t		get_time() const;
		bool		get_is_dead(void) const;
		bool		get_is_sending_data(void) const;
		std::string get_client_ip(void) const;
		bool 		get_is_new_data(void) const;
		size_t		get_onread_timeout() const;
		size_t		get_idle_timeout() const;
		size_t		get_max_size() const;
		size_t		get_max_request() const;
		size_t		get_process_data_size() const;
		Server_conf	get_conf() const;
};
