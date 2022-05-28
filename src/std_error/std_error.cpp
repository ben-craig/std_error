// std_error.cpp : Defines the entry point for the application.
//

#include <std_error/std_error.h>
#include <algorithm>
namespace err
{

namespace detail
{
out_buffer_description exception_error_domain__message(error_payload p, in_buffer_description in)
{
	out_buffer_description retval;

	std::exception_ptr eptr = *static_cast<std::exception_ptr*>(p.ptr);
	try {
		std::rethrow_exception(eptr);
	}
	catch (const std::exception& e) {
		const char* err_str = e.what();
		size_t len = strlen(err_str);
		if (len <= in.length) {
			std::copy(err_str, err_str + len + 1, in.buffer);
			retval.buffer = in.buffer;
		}
		else {
			retval.buffer = nullptr;
		}
		retval.length = len;
	}
	catch (...) {
		retval.buffer = "Unknown exception";
		retval.length = 0;
	}
	return retval;
}
}

error make_exception_error(const std::exception_ptr& e)
{
	error_payload p{ .ptr = new std::exception_ptr{e} };
	return error{ &exception_error_domain, p};
}

} // namespace err
