#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "util.hpp"

class Asserter
{
	private:
		bool exception;
		int number_of_test;
		int success;
		std::string actual_name;
		std::vector<std::string> tags;
		std::string actual_suite;
		std::string actual_comment;
		std::size_t	title_size;
		int number_suite;
		int success_suite;
		std::vector<bool> suite_mapped;
		bool ignore;

		/* selection */
		std::vector<std::string>	allowed_tags;
		std::vector<std::string>	allowed_suites;
		bool						exclusive_suite;

		void	verify_assert( bool condition )
		{
			if ( this->actual_suite == "" )
				this->print_error("You can't run a test outside a suite");
			if ( this->exception == true )
				this->print_test( false, "exception" );
			else
			{
				if ( condition )
				{
					this->success++;
					this->print_test( true );
				}
				else
					this->print_test( false );
			}
			this->number_of_test++;
		}

		void	print_test( bool success, std::string precision = "" )
		{
			std::cout << "\033[1;43;30m=\033[0m" << "\t";
			
			std::cout << ( success ? "\033[0;42m-OK" : "\033[0;41m-KO" ) << "\033[0m";
			std::string name = ( ( this->actual_name != "" ) ? this->actual_name : to_string_int( this->number_of_test ) );
			std::cout << " \033[1;37m[" << name << "]\033[0m";
			if ( this->actual_comment != "" )
			{
				std::cout << "\033[0;36m (" << this->actual_comment << ")\033[0m";
				this->actual_comment = "";
			}
			else if ( precision != "" )
				std::cout << "\033[0;36m (" << precision << ")\033[0m";
			this->print_tag();
			std::cout << std::endl;
		}

	public:
		Asserter( char **args ) :
			exception( false ),
			number_of_test(0),
			success(0),
			actual_name(""),
			tags( std::vector<std::string>() ),
			actual_suite(""),
			actual_comment(""),
			title_size(0),
			number_suite(0),
			success_suite(0),
			suite_mapped( std::vector<bool>() ),
			ignore( false ),
			allowed_tags( std::vector<std::string>() ),
			allowed_suites( std::vector<std::string>() ),
			exclusive_suite( false )
		{
			this->HEADER_MESSAGE();
			for ( int i = 0; args[i] != NULL; ++i )
			{
				std::string arg( args[i] );
				std::size_t s = arg.find( "=" );
				if ( s == std::string::npos )
					throw std::exception();
				std::string directive = arg.substr( 0, s );
				std::string value = arg.substr( s + 1, arg.size() );
				if ( value == "" )
					throw std::exception();
				if ( directive == "tag" )
					this->add_allowed_tag(value);
				else if ( directive == "suite" )
					this->add_allowed_suite(value);
			}
		}
		~Asserter()
		{
			if ( this->actual_suite != "" )
				this->print_error("A suite is still running at end.");
			std::cout << std::endl;
			std::cout << "\033[1;43;30mMapped result:\033[0m ";
			for ( std::vector<bool>::iterator it = this->suite_mapped.begin(); it != this->suite_mapped.end(); ++it )
			{
				std::string check;
				check += "\033[0;" + std::string( *it ? "42" : "41" ) + ";30m";
				check += "   ";
				check += "\033[0m";
				check += " ";
				std::cout << check;
			}
			std::cout << std::endl;
			std::cout << this->success_suite << " suite(s) pass over "
					<< this->number_suite << " suite(s)." << std::endl;
		}

		/* print */
		std::size_t	print_tag()
		{
			std::string res;
			std::size_t	size = 0;
			res += "\033[2;3;90m";
			for ( std::vector<std::string>::iterator it = this->tags.begin(); it != this->tags.end(); ++it )
			{
				if ( it != this->tags.begin() )
				{
					size += 1;
					res += ",";
				}
				res += " #" + *it;
				size += 2 + it->size();
			}
			res += "\033[0m";
			std::cout << res;
			return size;
		}

		void	print_suite()
		{
			std::cout << "\033[1;43;30m=\033[0m" << "\t";
			std::cout << std::endl;

			std::cout << "\033[1;43;30m=\033[0m" << "\t";

			std::cout << ( this->success == this->number_of_test ? "\033[0;42m" : "\033[0;41m" );

			std::cout << this->success << " test(s) pass over " << this->number_of_test
			<< " test(s)." << "\033[0m" << std::endl;

			std::cout << "\033[1;43;30m=\033[0m" << "\t";
			std::cout << std::endl;
			std::cout << "\033[1;43;30m";
			for ( std::size_t i = 0; i < this->title_size; ++i )
			{
				std::cout << "=";
			}
			std::cout << "\033[0m" << std::endl;
		}

		void	print_error( std::string msg, bool exception = true )
		{
			std::cout << "\033[1;31m" << msg << "\033[0m";
			this->print_tag();
			std::cout << std::endl;
			if ( exception )
				throw std::exception();
		}

		/* setter */
		void	throw_exception()
		{
			this->exception = true;
		}
		void	name( std::string name = "" )
		{
			this->actual_name = name;
		}
		void	add_tag( std::string tag )
		{
			this->tags.push_back( tag );
		}
		void	clear_tag()
		{
			this->tags.clear();
		}
		void	delete_last_tag()
		{
			this->tags.pop_back();
		}
		void	add_comment( std::string comment )
		{
			this->actual_comment = comment;
		}

		/* utils */
		std::size_t	advert( std::string str, int mode = 0 )
		{
			if ( mode == 0 )
				std::cout << std::endl << "\033[1;40m==>" << str << "<==\033[0m";
			else
				return 0;
			this->print_tag();
			std::cout << std::endl << std::endl;
			return 0;
		}
		void	suite_comment( std::string message )
		{
			if ( this->ignore == true || this->verify_tag() == false  )
				return ;
			std::cout << "\033[1;43;30m=\033[0m" << "\t";

			std::cout << "\033[1;40m==>" << message << "<==\033[0m";
			this->print_tag();
			std::cout << std::endl;
		}

		/* verify */
		void	verify_exception()
		{
			if ( this->ignore == true || this->verify_tag() == false  )
			{
				this->actual_comment = "";
				this->exception = false;
				return ;
			}
			if ( this->exception == true )
			{
				this->success++;
				this->print_test( true, "exception" );
			}
			else
				this->print_test( false, "no exception" );
			this->exception = false;
			this->number_of_test++;
		}
		template< class S >
		void	verify_assert_equal( S result, S expected )
		{
			if ( this->ignore == true || this->verify_tag() == false )
			{
				this->actual_comment = "";
				return ;
			}
			verify_assert( expected == result );
		}
		void	verify_assert_bool( bool condition )
		{
			if ( this->ignore == true || this->verify_tag() == false )
			{
				this->actual_comment = "";
				return ;
			}
			this->verify_assert( condition );
		}

		/* suite */
		void	new_suite( std::string suite_name )
		{
			this->exception = false;
			if ( suite_name == "" )
				this->print_error("Suit name cant be null.");
			if ( this->actual_suite != "" )
				this->print_error("You can't start a suite, an other suite is already running!");

			this->actual_suite = suite_name;

			this->exclusive_suite = this->verify_suite( suite_name ) && this->allowed_suites.size() > 0;
			if ( ( this->allowed_suites.size() > 0 || this->allowed_tags.size() > 0 ) &&
				this->exclusive_suite == false && ( this->verify_tag() == false || this->allowed_tags.size() == 0 ) )
			{
				this->ignore = true;
				return ;
			}

			std::cout << std::endl;
			std::string str = "Suite: " + suite_name;
			std::cout << "\033[1;43;30m======> " << str << " <======\033[0m";
			this->print_tag();
			std::cout << std::endl << "\033[1;43;30m=\033[0m" << std::endl;

			this->title_size = str.size() + 16;
			this->number_suite++;
		}

		void	end_suite()
		{
			this->exclusive_suite = false;
			this->actual_suite = "";
			if ( this->ignore == true )
			{
				this->ignore = false;
				return ;
			}
			print_suite();

			if ( this->success == this->number_of_test )
			{
				this->success_suite++;
				this->suite_mapped.push_back( true );
			}
			else
				this->suite_mapped.push_back( false );

			this->number_of_test = 0;
			this->success = 0;
			this->title_size = 0;
		}

		/* selection */
		void	add_allowed_suite( std::string suite_name )
		{
			this->allowed_suites.push_back( suite_name );
		}
		
		void	add_allowed_tag( std::string tag_name )
		{
			this->allowed_tags.push_back( tag_name );
		}

		bool	verify_tag()
		{
			return ( 
					this->exclusive_suite == true || 
					this->allowed_tags.size() == 0 || 
					std::find_first_of (allowed_tags.begin(), allowed_tags.end(),
                		tags.begin(), tags.end()) != allowed_tags.end()
				);
		}
		bool	verify_suite( std::string suite_name )
		{
			return ( this->allowed_suites.size() == 0 ||
					std::find( this->allowed_suites.begin(), this->allowed_suites.end(), suite_name ) != this->allowed_suites.end()
				);
		}

		void	HEADER_MESSAGE()
		{
			std::string str;

			str += "/******************************************************************************/\n";
			str += "/*                                                                            */\n";
			str += "/*                                                        :::      ::::::::   */\n";
			str += "/*   Unite tester for cpp                               :+:      :+:    :+:   */\n";
			str += "/*                                                    +:+ +:+         +:+     */\n";
			str += "/*   By: pleveque <pleveque@student.42.fr>          +#+  +:+       +#+        */\n";
			str += "/*                                                +#+#+#+#+#+   +#+           */\n";
			str += "/*                                                     #+#    #+#             */\n";
			str += "/*   Version: 1.0                 by pleveque         ###   ########.fr       */\n";
			str += "/*                                                                            */\n";
			str += "/******************************************************************************/";
			
			std::string res = "\033[1;43;37m";
			for ( std::string::iterator it = str.begin(); it != str.end(); ++it )
			{
				if ( *it != ' ' && *it != '\n' )
					res += *it;
				else
				{
					res += "\033[0m";
					res += *it;
					res += "\033[1;43;37m";
				}
			}
			res += "\033[0m";
			std::cout << res << std::endl;
		}
};
