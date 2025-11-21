#ifndef DATE_PARSER
#define DATE_PARSER

#include <string>
#include <utility>


#include "simple_error_struct.hpp"

struct Date {
	int year = 0;
	int month = 0;
	int day = 0;
	int hour = 0;
	int minute = 0;
	int second = 0;

	int weekDayNow = 0;

	// date as YYYY-MM-DD
	std::string ymd() {
		char buf[32];
		snprintf(buf, sizeof(buf), "%04d-%02d-%02d", year, month, day);

		return std::string(buf);
	}

	// date as YYYY-MM-DD HH:MM:SS
	std::string ymdhms() {
		char buf[64];
		snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, minute, second);

		return std::string(buf);
	}

	// Returns: 0 = Monday ... 6 = Sunday
	int weekday() {
		int new_year = year;
		int new_month = month;
    	if (new_month == 1) {       // January
    	    new_month = 13;
    	    new_year--;
    	} else if (new_month == 2) { // February
    	    new_month = 14;
    	    new_year--;
    	}

    	int q = day;
    	int m = new_month;
    	int K = new_year % 100;
    	int J = new_year / 100;

    	int h = (q + (13*(m + 1))/5 + K + K/4 + J/4 + 5*J) % 7;
		weekDayNow = (h+5)%7; 
    	return (h+5)%7; 
	}


};

// example ISO8601 date 2025-11-06T15:00:00Z
Date ISO8601DateParser(std::string dateStr, Error& err) {
	if (dateStr.length() < 19) {
		err.msg = "Date string was incorrect length";
		return Date();
	}

	Date out;
	out.year = std::stoi(dateStr.substr(0, 4));
	out.month = std::stoi(dateStr.substr(5, 2));
	out.day = std::stoi(dateStr.substr(8, 2));

	out.hour = std::stoi(dateStr.substr(11, 2));
	out.minute = std::stoi(dateStr.substr(14, 2));
	out.second = std::stoi(dateStr.substr(17, 2));

	return out;
}

Date ShortDateParser(std::string dateStr, Error& err) {
	if (dateStr.length() != 10) {
		err.msg = "Date string was incorrect length";
		return Date();
	}
	Date out;
	out.year = std::stoi(dateStr.substr(0, 4));
	out.month = std::stoi(dateStr.substr(5, 2));
	out.day = std::stoi(dateStr.substr(8, 2));
	return out;
}


Date unixToDate(uint64_t unixTimeAsMs) {
	unixTimeAsMs /= 1000;  // convert ms → seconds

	uint32_t days = unixTimeAsMs / 86400;
	uint32_t secsInDay = unixTimeAsMs % 86400;  //  remainder seconds within the day

	int hour = secsInDay / 3600;
	int minute = (secsInDay % 3600) / 60;
	int second = secsInDay % 60;

	// Howard Hinnant's date algorithm (public domain)
	days += 719468;
	int era = days / 146097;
	int doe = days - era * 146097;
	int yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
	int year = yoe + era * 400;
	int doy = doe - (365 * yoe + yoe / 4 - yoe / 100 + yoe / 400);
	int mp = (5 * doy + 2) / 153;
	int day = doy - (153 * mp + 2) / 5 + 1;
	int month = mp + (mp < 10 ? 3 : -9);
	year += (month <= 2);

	Date out;
	out.year = year;
	out.month = month;
	out.day = day;
	out.hour = hour;
	out.minute = minute;
	out.second = second;

	return out;
}

#endif