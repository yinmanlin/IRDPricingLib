#pragma once

#include <vector>
#include <string>
#include "OIS.h"

class LIBOR : public IDiscount
{
public:
    // read in data
    boost::gregorian::date today;
    std::vector<boost::gregorian::date> maturity;
    std::vector<std::string> dayCount;
    std::vector<std::string> source;
    std::vector<int> payFreq;
    std::vector<double> swapRates;
    
    // time, fwdrate
    std::vector<std::tuple<boost::gregorian::date, double>> curve;
    // zero rate to be implemented
    // 有点不会算
    //std::vector<double> zeroRates;
    
    // paras are today's date, tenor, libor file, ois file
    LIBOR(const boost::gregorian::date &, const double&, const std::string &, OIS&);
    void Calibrate(const boost::gregorian::date &, const double&, OIS&);
    double DF(const boost::gregorian::date &T, const std::string &);
    double fwd(const boost::gregorian::date &T, const std::string &);
};


    