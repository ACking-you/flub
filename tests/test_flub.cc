#include <doctest/doctest.h>
#include <flub/errors.h>

using namespace flub;

TEST_SUITE_BEGIN("Test for flub");

enum class ErrorCode { a, b, c, d };

struct ErrorDetail
{
   const char *msg;
   explicit ErrorDetail(const char *m) : msg(m) {}
   std::string to_string() const { return msg; }
};

TEST_CASE("boxed error")
{
   auto err   = Error<ErrorCode, ErrorDetail>{ErrorCode::a, ErrorDetail{"a"}};
   auto boxed = BoxedError::from(err);
   REQUIRE_EQ(boxed.is_type<ErrorCode>(), true);
   REQUIRE_EQ(boxed.to_enum<ErrorCode>(), ErrorCode::a);
   REQUIRE_EQ(boxed.string(), "a");
}

TEST_CASE("make error")
{
   auto err   = make_error(ErrorCode::a, ErrorDetail{"abc"});
   auto boxed = --err;
   REQUIRE_EQ(boxed.value().string(), "abc");
   auto ee = to_error<ErrorCode>(boxed);
   REQUIRE_EQ(ee.has_value(), true);
   REQUIRE_EQ((*ee).value().code, ErrorCode::a);
}

TEST_SUITE_END;