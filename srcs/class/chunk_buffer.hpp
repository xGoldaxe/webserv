#pragma once

#include <iostream>
#include <string>
#include "../errors/http_code.hpp"
#include "../utils/string.hpp"
#include "../utils/utils.hpp"

class Chunk_buffer
{
	protected:
		bool			valid;
		bool			head_valid;
		std::string		head;
		std::string		body;
		size_t			remain;
		/* limits */
		size_t MAX_HEAD_SIZE;
		size_t MAX_BODY_SIZE;

	public:
		/* coplien */
		Chunk_buffer( void )
		{
			valid = false;
			head_valid = false;
			head = "";
			body = "";
			remain = 0;
			MAX_HEAD_SIZE = 0;
			MAX_BODY_SIZE = 0;
		};
		~Chunk_buffer( void ){};
		// #ifdef DEBUG
			Chunk_buffer( bool valid, bool head_valid, std::string head, std::string body, size_t remain )
			:	valid( valid ),
				head_valid( head_valid ),
				head( head ),
				body( body ),
				remain( remain )
			{}
			bool	operator==( Chunk_buffer & rhs )
			{
				return (
					this->valid == rhs.valid &&
					this->head_valid == rhs.head_valid &&
					this->head == rhs.head &&
					this->body == rhs.body &&
					this->remain == rhs.remain
				);
			}
			void	clean( void )
			{
				this->valid = false;
				this->head_valid = false;
				this->head = "";
				this->body = "";
				this->remain = 0;
				this->MAX_HEAD_SIZE = 0;
				this->MAX_BODY_SIZE = 0;
			}
			void	print( void )
			{
				std::cout << "valid: " << valid << std::endl;
				std::cout << "head_valid: " << head_valid << std::endl;
				std::cout << "head: " << head << std::endl;
				std::cout << "body: " << body << std::endl;
				std::cout << "remain: " << remain << std::endl;
			}
		// #endif

		/* end coplien */
		std::string	get_body() const
		{
			if ( !is_valid() )
				return std::string();
			return this->body;
		}

		bool	is_last(void) const
		{
			return ( is_valid() && this->body.size() == 0 );
		}

		bool	is_valid(void) const {
			return this->valid;
		}

		void	set_limits( size_t head, size_t body )
		{
			this->MAX_HEAD_SIZE = head;
			this->MAX_BODY_SIZE = body;
		}

		std::string	feed_chunk( std::string raw )
		{
			if ( this->valid )
				return raw;
			//trunc raw for head
			if ( this->head_valid == false )
			{
				std::string tmp_head = test_through_str( raw, &check_line );
				if ( tmp_head.size() > this->MAX_HEAD_SIZE && this->MAX_HEAD_SIZE != 0 )
					throw HTTPCode413();
				if ( check_and_trunc_line( &tmp_head ) )
				{
					if ( tmp_head.size() == 0 || is_full_hexa( tmp_head ) == false ) // do not accept chunk extension!
						throw HTTPCode400();
					this->remain = hexToInt( tmp_head );
					if ( (this->remain > this->MAX_BODY_SIZE) && this->MAX_BODY_SIZE != 0 )
						throw HTTPCode413();
					this->head = tmp_head;
					this->head_valid = true;
					raw = std::string( raw.begin() + tmp_head.size() + 2, raw.end() );
				}
			}
			//trunc raw for body
			if ( this->head_valid == true && this->remain > 0 )
			{
				size_t size = raw.size() > this->remain ? this->remain : raw.size();
				this->body += raw.substr( 0, size );
				raw = raw.substr( size, raw.size() );
				this->remain -= size;
			}
			//trunc raw for final crlf
			if ( head_valid == true && this->remain == 0 )
			{
				if ( raw.size() >= 2 )
				{
					if ( raw[0] == '\r' && raw[1] == '\n' )
						this->valid = true;
					else
						throw HTTPCode400();
				}
			}
			return raw;
		}
};