#pragma once
#include "../webserv.hpp"

class Connection
{

	private:
		int			_fd;
		Request		_req;
		Response	*_res;
		std::string	_raw_data;
		bool		_is_init;
		time_t	 	_begin_time;
		const char	*_client_ip;
		
		Connection( void );

	public:

		/* coplien */
		Connection( int fd, char *client_ip );
		Connection( Connection const &src );
		~Connection( void );

		Connection &   operator=( Connection const & rhs );
		/* end coplien */
		bool	is_ready();
		bool	is_fulfilled();
		bool	is_invalid_req();
		void	add_data( char * buffer );
		bool	queue_iteration();
		void	process();
		void	soft_clear();
		bool	init_request();
		/* error case */
		bool	is_timeout(void);


		/* getter */
		int			get_fd() const;
		Request 	get_req() const;
		Response	*get_res() const;
		std::string	get_data() const;
		bool		is_init() const;
		time_t		get_time() const;
};