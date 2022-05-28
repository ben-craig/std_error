// std_error.cpp : Defines the entry point for the application.
//

#include <std_error_next/std_error2.h>

namespace err
{

error make_exception_error2(const std::exception_ptr& e)
{
	error_payload p{ .ptr = new std::exception_ptr{e} };
	return error{ &exception_error_domain2.domain1, p };
}

} // namespace err
