#ifndef TEST_DATE_PARSER
#define TEST_DATE_PARSER

#include <unity.h>
#include "dateparsing.hpp"
#include "simple_error_struct.hpp"

UnityTestFunction test_ISO8601DateParser() {
    Error err;
    Date date;

    std::string test_string = "2025-11-06T15:00:00Z";

    date = ISO8601DateParser(test_string, err);
    TEST_ASSERT(!err);
    TEST_ASSERT(date.year == 2025 && date.month == 11 && date.day == 06 && date.hour == 15 && date.minute == 00 && date.second == 00);

    test_string = "2025-11-06";
    TEST_ASSERT(err.msg == "Date string was incorrect length")
}

UnityTestFunction test_unix_date_parser() {
    Date date;
    date = unixToDate(1762430400000);
    TEST_ASSERT(date.ymdhms() == "2025-11-06 12:00:00");

}

#endif