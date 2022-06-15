#pragma once

#include <iostream>
#include <string>

bool	track_exception( bool mode, bool is_throw = false )
{
	static bool state = false;
	if ( mode == true )
		state = is_throw;
	return state;
}

#define ADD_TEST 0
#define ADD_SUCCESS 1
#define GET_TEST 2
#define GET_SUCCESS 3

int		result_info( int mode )
{
	static int number_of_test = 0;
	static int success = 0;
	if ( mode == ADD_TEST )
		number_of_test++;
	else if ( mode == ADD_SUCCESS )
		success++;
	else if ( mode == GET_TEST )
		return number_of_test;
	else if ( mode == GET_SUCCESS )
		return success;
	return 0;
}

void	verify_exception()
{
	if ( track_exception(false) == true )
	{
		result_info( ADD_SUCCESS );
		std::cout << "✅ [" << result_info( GET_TEST ) << "] -OK (exception)" << std::endl;
	}
	else
		std::cout << "❌ [" << result_info( GET_TEST ) << "] -KO (no exception)" << std::endl;
	track_exception( true, false );
	result_info( ADD_TEST );
}

void	verify_assert( bool condition );

template< class S >
void	verify_assert_equal( S result, S expected )
{
	// std::cout << "==begin debug==" << std::endl;
	// std::cout << "<<<<<<<<<< exepted >>>>>>>>>>" << std::endl;
	// expected.print();
	// std::cout << "<<<<<<<<<< result >>>>>>>>>>>" << std::endl;
	// result.print();
	// std::cout << "==end debug==" << std::endl;
	
	verify_assert( expected == result );
}

void	verify_assert_bool( bool condition )
{
	verify_assert( condition );
}

void	verify_assert( bool condition )
{
	bool success = true;
	bool exception = false;
	if ( track_exception(false) == true )
	{
		success = false;
		exception = true;
	}
	else
		success = condition;

	if ( success )
	{
		result_info( ADD_SUCCESS );
		std::cout << "✅ [" << result_info( GET_TEST ) << "] -OK" << std::endl;
	}
	else
	{
		std::cout << "❌ [" << result_info( GET_TEST ) << "] -KO" <<
		(exception ? " (exception)" : "") << std::endl;
	}
	result_info( ADD_TEST );
}