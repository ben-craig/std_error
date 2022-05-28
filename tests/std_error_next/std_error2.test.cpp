#include <std_error_next/std_error2.h>

#include <gtest/gtest.h>
#include <stdexcept>

// Demonstrate some basic assertions.
TEST(std_error2, check_old_error_for_new_stuff) {
	err::error e = err::make_exception_error(std::make_exception_ptr(std::runtime_error("error text")));
	EXPECT_EQ(nullptr, err::dynamic_error_domain_cast<err::error_domain2>(e.domain()));
};

TEST(std_error2, check_new_error_for_new_stuff) {
	err::error e = err::make_exception_error2(std::make_exception_ptr(std::runtime_error("error text2")));
	auto *new_domain = err::dynamic_error_domain_cast<err::error_domain2>(e.domain());
	EXPECT_EQ(new_domain->domain1.id, err::exception_error_domain2.domain1.id);
	auto& new_domain_ref = err::static_error_domain_cast<err::error_domain2>(*e.domain());
	EXPECT_EQ(new_domain_ref.domain1.id, err::exception_error_domain2.domain1.id);

	EXPECT_EQ(42, new_domain->dummy_extension_point());
	EXPECT_EQ(42, new_domain_ref.dummy_extension_point());
};
