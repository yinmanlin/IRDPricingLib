#include "DateUtil.h"


// different day count conventions
double durationWDaycount(const boost::gregorian::date &d1, const boost::gregorian::date &d2, const std::string &str)
{
    if (str == "ACT/360")
    {
        return (d2-d1).days()/360.0;
    }else if (str == "30I/360")
    {
        return ((d2.year()-d1.year())*360+(d2.month()-d1.month())*30+(d2.day()-d1.day()))/360.0;
    }else if (str == "ACT/365"){
        return (d2-d1).days()/365.0;
    }else
    {
        // by default is ACT/360
        return (d2-d1).days()/360.0;
    }
}


boost::gregorian::date returnWeekday(const boost::gregorian::date &d)
{
    if (d.day_of_week() == boost::date_time::Saturday)
    {
        return d + boost::gregorian::days(2);
    }else if (d.day_of_week() == boost::date_time::Sunday)
    {
        return d + boost::gregorian::days(1);
    }else{
        return d;
    }
}


// note here the paymentDates are in REVERSE order!!!!!
// note cannot match with bloomberg
// eg 2025, 4, 15 as maturity, 2020, 4, 15 as start
// cash flow day count convention???
std::vector<boost::gregorian::date> paymentDates(const boost::gregorian::date &start, const boost::gregorian::date &maturity, const int &payFreq)
{
    std::vector<boost::gregorian::date> payDates;
    // if payFreq is 0
    // not swap rate, just interest rate
    if (payFreq == 0)
    {
        payDates.push_back(maturity);
        payDates.push_back(start);
        return payDates;
    }else
    {
        int interval = 12 / payFreq;
        payDates.push_back(maturity);
        
        if (maturity - boost::gregorian::months(interval) > start + boost::gregorian::days(7))
        {
            // here not sure day count convention
            for (boost::gregorian::date i = maturity - boost::gregorian::months(interval); i > start + boost::gregorian::months(interval) - boost::gregorian::days((interval*30/6)); i = i - boost::gregorian::months(interval))
            {
                // check whether i is weekday or not
                i = returnWeekday(i);
                payDates.push_back(i);
            }
        }
        payDates.push_back(start);
        return payDates;
    }
}