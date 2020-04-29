#include <boost/date_time/gregorian/gregorian.hpp>
#include <string>

class IDiscount{
public:
    IDiscount();
    // discount factor starts from 0
    virtual double DF(const boost::gregorian::date &T, const std::string &) = 0;
};