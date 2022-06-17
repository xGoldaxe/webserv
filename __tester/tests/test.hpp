#pragma once

#include "../internal/assert_object.hpp"
#include "../../srcs/class/request.hpp"
#include "../../srcs/class/route.hpp"

void	demo_suite( Asserter &asserter );
void	all_tests( Asserter &asserter );
void	content_length_test( Asserter &asserter );
void	util_test( Asserter &asserter );
void	header_chunked( Asserter &asserter );
void	secure_chunked( Asserter &asserter, Request *req, std::string &raw );