#pragma once
#include <system_error>

namespace err
{

union error_payload {
  void *ptr = nullptr;
  int value;
};

struct in_buffer_description {
  char *buffer;
  size_t length;
};

struct out_buffer_description {
  const char *buffer;
  size_t length;
};

struct error_domain {
  static const int minimum_version = 0;
  unsigned long long id;
  void (*destroy_fn)(error_payload);
  out_buffer_description (*message_fn)(error_payload, in_buffer_description);
  const char *(*static_message_fn)(error_payload);
  std::errc (*flatten_fn)(error_payload);
  bool (*equal_fn)(error_payload, error_payload);
  int version = 0;
};

class error {
public:
  constexpr error() noexcept : d(nullptr) {}
  constexpr explicit error(const error_domain *domain, error_payload payload) noexcept
    : d(domain)
    , p(payload)
  {}
  constexpr error(error&& other) noexcept : d(other.d), p(other.p) {
      other.d = nullptr;
  }
  constexpr error &operator=(error&& other) noexcept {
      if (&other == this) {
          return *this;
      }
      d = other.d;
      p = other.p;
      other.d = nullptr;
      return *this;
  }
  constexpr ~error() {
      if (d)
          d->destroy_fn(p);
  }

  constexpr const error_domain* domain() const noexcept { return d; }
  constexpr error_payload payload() const noexcept { return p; }

  constexpr bool operator==(const error& e) const noexcept {
      if (d == nullptr || e.d == nullptr) {
          return d == e.d;
      }
      if (d->id != e.d->id) {
          return false;
      }
      decltype(d->equal_fn) equal_fn = d->equal_fn;
      if (e.d->version > d->version) {
          equal_fn = e.d->equal_fn;
      }
      return equal_fn(p, e.p);
  }
  constexpr bool operator!=(const error& e) const noexcept { return !(*this == e); }

  constexpr bool equivalent(const error& e) const noexcept {
      if (d == nullptr || e.d == nullptr) {
          return d == e.d;
      }
      if (d->id == e.d->id) {
          decltype(d->flatten_fn) flatten_fn = d->flatten_fn;
          if (e.d->version > d->version) {
              flatten_fn = e.d->flatten_fn;
          }
          return flatten_fn(p) == flatten_fn(e.p);
      }
      return d->flatten_fn(p) == e.d->flatten_fn(e.p);
  }
private:
  const error_domain *d;
  error_payload p;
};

// convenience function
std::string error_message(const error &e);

namespace detail
{
out_buffer_description exception_error_domain__message(error_payload p, in_buffer_description in);
}

const inline unsigned long long exception_error_domain_id = 0xb88fda8c86ea45e3ull;

constexpr inline error_domain exception_error_domain = {
  .id = exception_error_domain_id,
  .destroy_fn = [](error_payload p) {
    delete static_cast<std::exception_ptr*>(p.ptr);
  },
  .message_fn = detail::exception_error_domain__message,
  .static_message_fn = [](error_payload) {return "exception error"; },
  .flatten_fn = [](error_payload) {return std::errc::bad_address; },
  .equal_fn = [](error_payload lhs, error_payload rhs) {
      std::exception_ptr* elhs = static_cast<std::exception_ptr*>(lhs.ptr);
      std::exception_ptr* erhs = static_cast<std::exception_ptr*>(rhs.ptr);
      return *elhs == *erhs;
  },
  .version = 0,
};

error make_exception_error(const std::exception_ptr& e = std::current_exception());

} // namespace err
