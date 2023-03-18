// Copyright (C) 2020  Matthew "strager" Glazar
// See end of file for extended copyright information.

#include <cstddef>
#include <cstring>
#include <gtest/gtest.h>
#include <quick-lint-js/port/integer.h>
#include <quick-lint-js/port/warning.h>
#include <string_view>
#include <system_error>

QLJS_WARNING_IGNORE_GCC("-Wsuggest-override")
QLJS_WARNING_IGNORE_GCC("-Wtype-limits")

using namespace std::literals::string_view_literals;

namespace quick_lint_js {
namespace {
using test_integer_from_chars_decimal_types =
    ::testing::Types<unsigned short, int, std::size_t>;
template <class T>
class test_integer_from_chars_decimal : public ::testing::Test {};
TYPED_TEST_SUITE(test_integer_from_chars_decimal,
                 test_integer_from_chars_decimal_types,
                 ::testing::internal::DefaultNameGenerator);

TYPED_TEST(test_integer_from_chars_decimal, common_non_negative_integers) {
  {
    TypeParam number;
    const char *input = "0";
    from_chars_result result =
        from_chars(input, input + std::strlen(input), number);
    EXPECT_EQ(number, 0);
    EXPECT_EQ(result.ptr, input + std::strlen(input));
    EXPECT_EQ(result.ec, std::errc{0});
  }

  {
    TypeParam number;
    const char *input = "1234";
    from_chars_result result =
        from_chars(input, input + std::strlen(input), number);
    EXPECT_EQ(number, 1234);
    EXPECT_EQ(result.ptr, input + std::strlen(input));
    EXPECT_EQ(result.ec, std::errc{0});
  }
}

TEST(test_integer_from_wchars_decimal_unsigned_short, common_integers) {
  {
    unsigned short number;
    const wchar_t *input = L"1234";
    from_wchars_result result =
        from_chars(input, input + std::wcslen(input), number);
    EXPECT_EQ(number, 1234);
    EXPECT_EQ(result.ptr, input + std::wcslen(input));
    EXPECT_EQ(result.ec, std::errc{0});
  }
}

TEST(test_integer_from_chars_decimal_int, common_negative_integers) {
  {
    int number;
    const char *input = "-1234";
    from_chars_result result =
        from_chars(input, input + std::strlen(input), number);
    EXPECT_EQ(number, -1234);
    EXPECT_EQ(result.ptr, input + std::strlen(input));
    EXPECT_EQ(result.ec, std::errc{0});
  }
}

TEST(test_integer_from_chars_decimal_int, minimum_integer) {
  static_assert(std::numeric_limits<int>::min() == -2147483648LL);
  int number;
  const char *input = "-2147483648";
  from_chars_result result =
      from_chars(input, input + std::strlen(input), number);
  EXPECT_EQ(number, -2147483648LL);
  EXPECT_EQ(result.ptr, input + std::strlen(input));
  EXPECT_EQ(result.ec, std::errc{0});
}

TEST(test_integer_from_chars_decimal_unsigned_short, maximum_integer) {
  static_assert(std::numeric_limits<unsigned short>::max() == 65535);
  unsigned short number;
  const char *input = "65535";
  from_chars_result result =
      from_chars(input, input + std::strlen(input), number);
  EXPECT_EQ(number, 65535);
  EXPECT_EQ(result.ptr, input + std::strlen(input));
  EXPECT_EQ(result.ec, std::errc{0});
}

TEST(test_integer_from_chars_decimal_int, maximum_integer) {
  static_assert(std::numeric_limits<int>::max() == 2147483647);
  int number;
  const char *input = "2147483647";
  from_chars_result result =
      from_chars(input, input + std::strlen(input), number);
  EXPECT_EQ(number, 2147483647);
  EXPECT_EQ(result.ptr, input + std::strlen(input));
  EXPECT_EQ(result.ec, std::errc{0});
}

TEST(test_integer_from_chars_decimal_size_t, maximum_integer) {
  static_assert(std::numeric_limits<std::size_t>::max() == 4294967295ULL ||
                std::numeric_limits<std::size_t>::max() ==
                    18446744073709551615ULL);

  {
    std::size_t number;
    const char *input = "4294967295";
    from_chars_result result =
        from_chars(input, input + std::strlen(input), number);
    EXPECT_EQ(number, 4294967295ULL);
    EXPECT_EQ(result.ptr, input + std::strlen(input));
    EXPECT_EQ(result.ec, std::errc{0});
  }

  if (std::numeric_limits<std::size_t>::max() >= 18446744073709551615ULL) {
    std::size_t number;
    const char *input = "18446744073709551615";
    from_chars_result result =
        from_chars(input, input + std::strlen(input), number);
    EXPECT_EQ(number, 18446744073709551615ULL);
    EXPECT_EQ(result.ptr, input + std::strlen(input));
    EXPECT_EQ(result.ec, std::errc{0});
  }
}

TEST(test_integer_from_chars_decimal_unsigned_short, over_maximum_integer) {
  static_assert(std::numeric_limits<unsigned short>::max() < 65536);

  {
    unsigned short number = 42;
    const char *input = "65536";
    from_chars_result result =
        from_chars(input, input + std::strlen(input), number);
    EXPECT_EQ(result.ptr, input + std::strlen(input));
    EXPECT_EQ(result.ec, std::errc::result_out_of_range);
    EXPECT_EQ(number, 42) << "number should be unmodified";
  }

  {
    unsigned short number = 42;
    const char *input = "9999999999999999999";
    from_chars_result result =
        from_chars(input, input + std::strlen(input), number);
    EXPECT_EQ(result.ptr, input + std::strlen(input));
    EXPECT_EQ(result.ec, std::errc::result_out_of_range);
    EXPECT_EQ(number, 42) << "number should be unmodified";
  }
}

TEST(test_integer_from_chars_decimal_int, over_maximum_integer) {
  static_assert(std::numeric_limits<int>::max() < 2147483648LL);

  {
    int number = 42;
    const char *input = "2147483648";
    from_chars_result result =
        from_chars(input, input + std::strlen(input), number);
    EXPECT_EQ(result.ptr, input + std::strlen(input));
    EXPECT_EQ(result.ec, std::errc::result_out_of_range);
    EXPECT_EQ(number, 42) << "number should be unmodified";
  }

  {
    int number = 42;
    const char *input = "9999999999999999999";
    from_chars_result result =
        from_chars(input, input + std::strlen(input), number);
    EXPECT_EQ(result.ptr, input + std::strlen(input));
    EXPECT_EQ(result.ec, std::errc::result_out_of_range);
    EXPECT_EQ(number, 42) << "number should be unmodified";
  }
}

TEST(test_integer_from_chars_decimal_size_t, over_maximum_integer) {
  static_assert(std::numeric_limits<std::size_t>::max() == 4294967295ULL ||
                std::numeric_limits<std::size_t>::max() ==
                    18446744073709551615ULL);

  if (std::numeric_limits<std::size_t>::max() <= 4294967295ULL) {
    std::size_t number = 42;
    const char *input = "4294967296";
    from_chars_result result =
        from_chars(input, input + std::strlen(input), number);
    EXPECT_EQ(result.ptr, input + std::strlen(input));
    EXPECT_EQ(result.ec, std::errc::result_out_of_range);
    EXPECT_EQ(number, 42) << "number should be unmodified";
  }

  {
    std::size_t number = 42;
    const char *input = "18446744073709551616";
    from_chars_result result =
        from_chars(input, input + std::strlen(input), number);
    EXPECT_EQ(result.ptr, input + std::strlen(input));
    EXPECT_EQ(result.ec, std::errc::result_out_of_range);
    EXPECT_EQ(number, 42) << "number should be unmodified";
  }

  {
    int number = 42;
    const char *input = "9999999999999999999999";
    from_chars_result result =
        from_chars(input, input + std::strlen(input), number);
    EXPECT_EQ(result.ptr, input + std::strlen(input));
    EXPECT_EQ(result.ec, std::errc::result_out_of_range);
    EXPECT_EQ(number, 42) << "number should be unmodified";
  }
}

TEST(test_integer_from_chars_decimal_size_t, negative_integers_are_disallowed) {
  {
    std::size_t number = 42;
    const char *input = "-9001";
    from_chars_result result =
        from_chars(input, input + std::strlen(input), number);
    EXPECT_EQ(result.ptr, input);
    EXPECT_EQ(result.ec, std::errc{std::errc::invalid_argument});
    EXPECT_EQ(number, 42) << "number should be unmodified";
  }
}

TYPED_TEST(test_integer_from_chars_decimal,
           extra_characters_after_are_not_parsed) {
  {
    TypeParam number;
    const char *input = "1234abcd";
    from_chars_result result =
        from_chars(input, input + std::strlen(input), number);
    EXPECT_EQ(number, 1234);
    EXPECT_EQ(result.ptr, &input[4]);
    EXPECT_EQ(result.ec, std::errc{0});
  }

  {
    TypeParam number;
    const char *input = "123   ";
    from_chars_result result =
        from_chars(input, input + std::strlen(input), number);
    EXPECT_EQ(number, 123);
    EXPECT_EQ(result.ptr, &input[3]);
    EXPECT_EQ(result.ec, std::errc{0});
  }
}

TYPED_TEST(test_integer_from_chars_decimal, extra_characters_before) {
  {
    TypeParam number = 42;
    const char *input = "  123";
    from_chars_result result =
        from_chars(input, input + std::strlen(input), number);
    EXPECT_EQ(result.ptr, input);
    EXPECT_EQ(result.ec, std::errc{std::errc::invalid_argument});
    EXPECT_EQ(number, 42) << "number should be unmodified";
  }

  {
    TypeParam number = 42;
    const char *input = "--123";
    from_chars_result result =
        from_chars(input, input + std::strlen(input), number);
    EXPECT_EQ(result.ptr, input);
    EXPECT_EQ(result.ec, std::errc{std::errc::invalid_argument});
    EXPECT_EQ(number, 42) << "number should be unmodified";
  }

  {
    TypeParam number = 42;
    const char *input = "+123";
    from_chars_result result =
        from_chars(input, input + std::strlen(input), number);
    EXPECT_EQ(result.ptr, input);
    EXPECT_EQ(result.ec, std::errc{std::errc::invalid_argument});
    EXPECT_EQ(number, 42) << "number should be unmodified";
  }
}

TYPED_TEST(test_integer_from_chars_decimal, radix_prefix_is_not_special) {
  {
    TypeParam number;
    const char *input = "0x123a";
    from_chars_result result =
        from_chars(input, input + std::strlen(input), number);
    EXPECT_EQ(number, 0);
    EXPECT_EQ(result.ptr, &input[1]);
    EXPECT_EQ(result.ec, std::errc{0});
  }

  {
    TypeParam number;
    const char *input = "0777";
    from_chars_result result =
        from_chars(input, input + std::strlen(input), number);
    EXPECT_EQ(number, 777);
    EXPECT_EQ(result.ptr, input + std::strlen(input));
    EXPECT_EQ(result.ec, std::errc{0});
  }
}

TYPED_TEST(test_integer_from_chars_decimal,
           empty_input_string_is_unrecognized) {
  TypeParam number = 42;
  const char *input = "";
  from_chars_result result = from_chars(input, input, number);
  EXPECT_EQ(result.ptr, input);
  EXPECT_EQ(result.ec, std::errc::invalid_argument);
  EXPECT_EQ(number, 42) << "number should be unmodified";
}

TYPED_TEST(test_integer_from_chars_decimal,
           minus_sign_without_digits_is_unrecognized) {
  TypeParam number = 42;
  const char *input = "- 1";
  from_chars_result result =
      from_chars(input, input + std::strlen(input), number);
  EXPECT_EQ(result.ptr, input);
  EXPECT_EQ(result.ec, std::errc::invalid_argument);
  EXPECT_EQ(number, 42) << "number should be unmodified";
}

using test_parse_number_exact_decimal_types =
    ::testing::Types<unsigned short, int, std::size_t>;
template <class T>
class test_parse_number_exact_decimal : public ::testing::Test {};
TYPED_TEST_SUITE(test_parse_number_exact_decimal,
                 test_parse_number_exact_decimal_types,
                 ::testing::internal::DefaultNameGenerator);

TYPED_TEST(test_parse_number_exact_decimal, common_non_negative_integers) {
  {
    TypeParam number;
    parse_number_exact_error parse_error = parse_number_exact("0"sv, number);
    EXPECT_EQ(parse_error, parse_number_exact_error::ok);
    EXPECT_EQ(number, 0);
  }

  {
    TypeParam number;
    parse_number_exact_error parse_error = parse_number_exact("1234"sv, number);
    EXPECT_EQ(parse_error, parse_number_exact_error::ok);
    EXPECT_EQ(number, 1234);
  }
}

TEST(test_parse_number_exact_wchars_decimal_unsigned_short, common_integers) {
  {
    unsigned short number;
    parse_number_exact_error parse_error =
        parse_number_exact(L"1234"sv, number);
    EXPECT_EQ(parse_error, parse_number_exact_error::ok);
    EXPECT_EQ(number, 1234);
  }
}

TEST(test_parse_number_exact_decimal_int, common_negative_integers) {
  {
    int number;
    parse_number_exact_error parse_error =
        parse_number_exact("-1234"sv, number);
    EXPECT_EQ(parse_error, parse_number_exact_error::ok);
    EXPECT_EQ(number, -1234);
  }
}

TEST(test_parse_number_exact_decimal_int, minimum_integer) {
  static_assert(std::numeric_limits<int>::min() == -2147483648LL);
  int number;
  parse_number_exact_error parse_error =
      parse_number_exact("-2147483648"sv, number);
  EXPECT_EQ(parse_error, parse_number_exact_error::ok);
  EXPECT_EQ(number, -2147483648LL);
}

TEST(test_parse_number_exact_decimal_unsigned_short, maximum_integer) {
  static_assert(std::numeric_limits<unsigned short>::max() == 65535);
  unsigned short number;
  parse_number_exact_error parse_error = parse_number_exact("65535"sv, number);
  EXPECT_EQ(number, 65535);
  EXPECT_EQ(parse_error, parse_number_exact_error::ok);
}

TEST(test_parse_number_exact_decimal_int, maximum_integer) {
  static_assert(std::numeric_limits<int>::max() == 2147483647);
  int number;
  parse_number_exact_error parse_error =
      parse_number_exact("2147483647"sv, number);
  EXPECT_EQ(number, 2147483647);
  EXPECT_EQ(parse_error, parse_number_exact_error::ok);
}

TEST(test_parse_number_exact_decimal_size_t, maximum_integer) {
  static_assert(std::numeric_limits<std::size_t>::max() == 4294967295ULL ||
                std::numeric_limits<std::size_t>::max() ==
                    18446744073709551615ULL);

  {
    std::size_t number;
    parse_number_exact_error parse_error =
        parse_number_exact("4294967295"sv, number);
    EXPECT_EQ(number, 4294967295ULL);
    EXPECT_EQ(parse_error, parse_number_exact_error::ok);
  }

  if (std::numeric_limits<std::size_t>::max() >= 18446744073709551615ULL) {
    std::size_t number;
    parse_number_exact_error parse_error =
        parse_number_exact("18446744073709551615"sv, number);
    EXPECT_EQ(number, 18446744073709551615ULL);
    EXPECT_EQ(parse_error, parse_number_exact_error::ok);
  }
}

TEST(test_parse_number_exact_decimal_unsigned_short, over_maximum_integer) {
  static_assert(std::numeric_limits<unsigned short>::max() < 65536);

  {
    unsigned short number = 42;
    parse_number_exact_error parse_error =
        parse_number_exact("65536"sv, number);
    EXPECT_EQ(parse_error, parse_number_exact_error::out_of_range);
    EXPECT_EQ(number, 42) << "number should be unmodified";
  }

  {
    unsigned short number = 42;
    parse_number_exact_error parse_error =
        parse_number_exact("9999999999999999999"sv, number);
    EXPECT_EQ(parse_error, parse_number_exact_error::out_of_range);
    EXPECT_EQ(number, 42) << "number should be unmodified";
  }
}

TEST(test_parse_number_exact_decimal_int, over_maximum_integer) {
  static_assert(std::numeric_limits<int>::max() < 2147483648LL);

  {
    int number = 42;
    parse_number_exact_error parse_error =
        parse_number_exact("2147483648"sv, number);
    EXPECT_EQ(parse_error, parse_number_exact_error::out_of_range);
    EXPECT_EQ(number, 42) << "number should be unmodified";
  }

  {
    int number = 42;
    parse_number_exact_error parse_error =
        parse_number_exact("9999999999999999999"sv, number);
    EXPECT_EQ(parse_error, parse_number_exact_error::out_of_range);
    EXPECT_EQ(number, 42) << "number should be unmodified";
  }
}

TEST(test_parse_number_exact_decimal_size_t, over_maximum_integer) {
  static_assert(std::numeric_limits<std::size_t>::max() == 4294967295ULL ||
                std::numeric_limits<std::size_t>::max() ==
                    18446744073709551615ULL);

  if (std::numeric_limits<std::size_t>::max() <= 4294967295ULL) {
    std::size_t number = 42;
    parse_number_exact_error parse_error =
        parse_number_exact("4294967296"sv, number);
    EXPECT_EQ(parse_error, parse_number_exact_error::out_of_range);
    EXPECT_EQ(number, 42) << "number should be unmodified";
  }

  {
    std::size_t number = 42;
    parse_number_exact_error parse_error =
        parse_number_exact("18446744073709551616"sv, number);
    EXPECT_EQ(parse_error, parse_number_exact_error::out_of_range);
    EXPECT_EQ(number, 42) << "number should be unmodified";
  }

  {
    int number = 42;
    parse_number_exact_error parse_error =
        parse_number_exact("9999999999999999999999"sv, number);
    EXPECT_EQ(parse_error, parse_number_exact_error::out_of_range);
    EXPECT_EQ(number, 42) << "number should be unmodified";
  }
}

TEST(test_parse_number_exact_decimal_size_t, negative_integers_are_disallowed) {
  {
    std::size_t number = 42;
    parse_number_exact_error parse_error =
        parse_number_exact("-9001"sv, number);
    EXPECT_EQ(parse_error, parse_number_exact_error::invalid);
    EXPECT_EQ(number, 42) << "number should be unmodified";
  }
}

TYPED_TEST(test_parse_number_exact_decimal,
           extra_characters_after_are_not_parsed) {
  {
    TypeParam number = 42;
    parse_number_exact_error parse_error =
        parse_number_exact("1234abcd"sv, number);
    EXPECT_EQ(parse_error, parse_number_exact_error::invalid);
    EXPECT_EQ(number, 42) << "number should be unmodified";
  }

  {
    TypeParam number = 42;
    parse_number_exact_error parse_error =
        parse_number_exact("123   "sv, number);
    EXPECT_EQ(parse_error, parse_number_exact_error::invalid);
    EXPECT_EQ(number, 42) << "number should be unmodified";
  }
}

TYPED_TEST(test_parse_number_exact_decimal, extra_characters_before) {
  {
    TypeParam number = 42;
    parse_number_exact_error parse_error =
        parse_number_exact("  123"sv, number);
    EXPECT_EQ(parse_error, parse_number_exact_error::invalid);
    EXPECT_EQ(number, 42) << "number should be unmodified";
  }

  {
    TypeParam number = 42;
    parse_number_exact_error parse_error =
        parse_number_exact("--123"sv, number);
    EXPECT_EQ(parse_error, parse_number_exact_error::invalid);
    EXPECT_EQ(number, 42) << "number should be unmodified";
  }

  {
    TypeParam number = 42;
    parse_number_exact_error parse_error = parse_number_exact("+123"sv, number);
    EXPECT_EQ(parse_error, parse_number_exact_error::invalid);
    EXPECT_EQ(number, 42) << "number should be unmodified";
  }
}

TYPED_TEST(test_parse_number_exact_decimal, radix_prefix_is_not_special) {
  {
    TypeParam number = 42;
    parse_number_exact_error parse_error =
        parse_number_exact("0x123a"sv, number);
    EXPECT_EQ(parse_error, parse_number_exact_error::invalid);
    EXPECT_EQ(number, 42) << "number should be unmodified";
  }

  {
    TypeParam number;
    parse_number_exact_error parse_error = parse_number_exact("0777"sv, number);
    EXPECT_EQ(parse_error, parse_number_exact_error::ok);
    EXPECT_EQ(number, 777);
  }
}

TYPED_TEST(test_parse_number_exact_decimal,
           empty_input_string_is_unrecognized) {
  TypeParam number = 42;
  parse_number_exact_error parse_error = parse_number_exact(""sv, number);
  EXPECT_EQ(parse_error, parse_number_exact_error::invalid);
  EXPECT_EQ(number, 42) << "number should be unmodified";
}

TYPED_TEST(test_parse_number_exact_decimal,
           minus_sign_without_digits_is_unrecognized) {
  TypeParam number = 42;
  parse_number_exact_error parse_error = parse_number_exact("- 1"sv, number);
  EXPECT_EQ(parse_error, parse_number_exact_error::invalid);
  EXPECT_EQ(number, 42) << "number should be unmodified";
}
}
}

// quick-lint-js finds bugs in JavaScript programs.
// Copyright (C) 2020  Matthew "strager" Glazar
//
// This file is part of quick-lint-js.
//
// quick-lint-js is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// quick-lint-js is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with quick-lint-js.  If not, see <https://www.gnu.org/licenses/>.
