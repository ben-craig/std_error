#pragma once

#include <std_error/std_error.h>

namespace err
{

struct error_domain2 {
  static const int minimum_version = 1;
  error_domain domain1;
  int (*dummy_extension_point)();
};

template <typename T>
const T &static_error_domain_cast(const error_domain &d) {
  (void)T::minimum_version;
  return reinterpret_cast<const T &>(d);
}

template <typename T>
const T *dynamic_error_domain_cast(const error_domain *d) {
  if(d->version >= T::minimum_version)
    return reinterpret_cast<const T *>(d);
  return nullptr;
}

namespace detail {
	constexpr error_domain2 init_exception_error_domain2()
	{
		error_domain2 retval{
			.domain1 = exception_error_domain,
			.dummy_extension_point = [] {return 42; }
		};
		retval.domain1.version = 1;
		retval.domain1.flatten_fn = [](error_payload) {return std::errc::address_family_not_supported; };
		return retval;
	}
}

constexpr inline error_domain2 exception_error_domain2 = detail::init_exception_error_domain2();

error make_exception_error2(const std::exception_ptr& e = std::current_exception());

}
