#pragma once

#include <string>
#include <vector>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time.hpp>

// returns d2 - d1 in years ( in decimals )
double durationWDaycount(const boost::gregorian::date &d1, const boost::gregorian::date &d2, const std::string &str);

// if passed in date is not weekday return the next weekday
boost::gregorian::date returnWeekday(const boost::gregorian::date &d);

// return a vector of payment dates
std::vector<boost::gregorian::date> paymentDates(const boost::gregorian::date &, const boost::gregorian::date &, const int &);

