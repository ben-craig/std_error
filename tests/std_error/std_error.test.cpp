#include <std_error/std_error.h>

#include <gtest/gtest.h>
#include <stdexcept>

// Demonstrate some basic assertions.
TEST(std_error, exception_error_basics) {
	err::error e = err::make_exception_error(std::make_exception_ptr(std::runtime_error("error text")));

	EXPECT_EQ(e.domain()->id, err::exception_error_domain_id);
	EXPECT_EQ(e.domain()->version, 0);
	EXPECT_NE(e.payload().ptr, nullptr);
	EXPECT_STREQ(e.domain()->static_message_fn(e.payload()), "exception error");
	char buffer[1024];
	err::in_buffer_description desc{ .buffer = buffer, .length = std::size(buffer) };
	auto out = e.domain()->message_fn(e.payload(), desc);
	EXPECT_STREQ(out.buffer, "error text");
	EXPECT_EQ(out.length, strlen("error text"));
	EXPECT_EQ(out.buffer, buffer);

	desc.length = 0;
	out = e.domain()->message_fn(e.payload(), desc);
	EXPECT_EQ(out.length, strlen("error text"));
	EXPECT_EQ(out.buffer, nullptr);
}
