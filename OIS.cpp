#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "OIS.h"


OIS::OIS(const boost::gregorian::date &t, const std::string &filename) : IDiscount()
{
    today = t;
    std::ifstream w;
    w.open(filename);
    std::string readline, substring;
    getline(w, readline); //remove header
    double p1, p2;
    while(getline(w, readline))
    {
        std::stringstream ss(readline);
        getline(ss, substring, ',');
        p1 = std::stod(substring);
        getline(ss, substring, ',');
        p2 = std::stod(substring);
        // push back mid market quote rate in percentage
        swapRates.push_back((p1 + p2) / 200.0);
        getline(ss, substring, ',');
        dayCount.push_back(substring);
        getline(ss, substring, ',');
        payFreq.push_back(std::stoi(substring));
        getline(ss, substring, ',');
        // remove possible \r otherwise boost cannot recgonize
        if (!substring.empty() && substring[substring.size() - 1] == '\r')
            substring.erase(substring.size() - 1);
        maturity.push_back(boost::gregorian::from_us_string(substring));
    }
    // checked data read in are all correct
    Calibrate();
}


void OIS::Calibrate()
{
    // first build yield curve
    // iterate through every swap contract
    for (int i = 0; i <= maturity.size() - 1; ++i)
    {
        //std::cout << maturity[i] << "\n";
        
        std::vector<boost::gregorian::date> payDates = paymentDates(today, maturity[i], payFreq[i]);
        
        //for (auto & e : payDates)
        //{
        //    std::cout << e << " ";
        //}
        //std::cout << "\n";
        
        // if there is only one cash flow exchange
        if(payDates.size() == 2)
        {
            // here calculates df using simple interest rate
            double df = 1.0/(1+swapRates[i]*durationWDaycount(today, maturity[i], dayCount[i]));
            // zero rate calculation ???
            auto t = std::make_tuple(maturity[i], swapRates[i], df);
            curve.push_back(t);
        }else
        {
            // skip the last element because it's the start date
            // goal here is to get df for every payment dates
            // tempRate is in ordinary order
            std::vector<double> tempRate;
            for (auto i = payDates.rbegin() + 1; i != payDates.rend() - 1; ++i )
            {
                //std::cout << "\n";
                //std::cout << *i << "\n";
                // if cash flow dates already present in curve vector
                auto it = std::find_if(curve.begin(), curve.end(),
                                       [&i](const std::tuple<boost::gregorian::date, double,double>& e){ return std::get<0>(e) == *i;} );
                if (it != curve.end())
                {
                    //std::cout << "no interpolation\n";
                    auto index = std::distance(curve.begin(), it);
                    //std::cout << std::get<1>(curve[index]) << "\n";
                    tempRate.push_back(std::get<1>(curve[index]));
                } else
                {
                    // interpolate
                    //std::cout << "interpolation\n";
                    auto r = interpolation(*i, curve, "linear");
                    //std::cout << r << "\n";
                    tempRate.push_back(r);
                }
            }
            
            // for debug
            //std::cout << "tempRate:\n";
            //for (auto & e : tempRate)
            //{
            //    std::cout << e << " ";
            //}
            //std::cout << "\n";
            
            // now have all necessary rates to compute zero rate
            // tempRate is a vector of size = payDates.size()-2
            // order of payDates and tempRate are reverse
            double latestDF = 0.0;
            double numerator = 0.0;
            int rateIndex = 0;
            
            //对于reverse +是从后往前 -是从前往后
            // 从payDates的倒数第二个元素循环到第二个元素
            for (auto j = payDates.rbegin() + 1; j != payDates.rend() - 1; ++j )
            {
                //discount factor from start to paydate
                double df = 1 / (1 + durationWDaycount(payDates[payDates.size()-1], *j, dayCount[i]) * tempRate[rateIndex]);
                numerator += durationWDaycount(*(j-1), *j, dayCount[i]) * swapRates[i] * df;
                ++rateIndex;
            }
            double denominator = 1 + swapRates[i] * durationWDaycount(payDates[1], payDates[0], dayCount[i]);
            latestDF = (1 - numerator)/denominator;
            double latestZero = (1.0 / latestDF - 1) / durationWDaycount(today, maturity[i], dayCount[i]);
            // zero rate calculation ???
            auto t = std::make_tuple(maturity[i], latestZero, latestDF);
            curve.push_back(t);
        }
    }
    // calculate fwd rate last
    // first forward rate is spot zero rate
    // ????????????????
    // which day count should use when calculating fwdrate ?????
    // ????????????????
    fwdRates.push_back(std::get<1>(curve[0]));
    double fwdProduct = 1 + fwdRates[0] * durationWDaycount(today,std::get<0>(curve[0]), "ACT/360");
    for (int i = 1; i <= curve.size() - 1; ++i)
    {
        double df = std::get<2>(curve[i]);
        double fwdR = (1/(df * fwdProduct) - 1) / durationWDaycount(std::get<0>(curve[i-1]),std::get<0>(curve[i]), "ACT/360");
        fwdRates.push_back(fwdR);
        fwdProduct = fwdProduct * (1 + fwdRates[i] * durationWDaycount(std::get<0>(curve[i-1]),std::get<0>(curve[i]), "ACT/360"));
    }
}

double OIS::zero(const boost::gregorian::date &T, const std::string &method)
{
    // sort curve according to date
    sort(curve.begin(), curve.end());
    // identify two closest dates and corresponding rates
    auto itr = std::lower_bound(curve.begin(), curve.end(), std::make_tuple(T,1.0,1.0),
                                [](std::tuple<boost::gregorian::date, double, double> lhs, std::tuple<boost::gregorian::date, double, double> rhs) -> bool { return std::get<0>(lhs) < std::get<0>(rhs); });
    if (method == "linear")
    {
        if (itr == curve.end())
        {
            // 这里如果超过了期限 return了最近的利率 并没有写extrapolation
            // 可以改进
            //std::cout << "Input date out of max range, latest date in yield curve is " << std::get<0>(curve[curve.size()-1]) << ". Returned latest zero rate.\n";
            return std::get<1>(curve[curve.size()-1]);
        }else
        {
            auto itl = itr - 1;
            // right date
            auto dt2 = std::get<0>(*itr);
            // left date
            auto dt1 = std::get<0>(*itl);
            // right zero rate
            auto r2 = std::get<1>(*itr);
            // left zero rate
            auto r1 = std::get<1>(*itl);
            double t1 = (T - dt1).days();
            double t2 = (dt2 - dt1).days();
            return r1 + t1/t2 * (r2-r1);
        }
    }else
    {
        // other interpolation methods to be implemented
        return 0.0;
    }
}



double OIS::DF(const boost::gregorian::date &T, const std::string &method)
{
    return 1/(1+zero(T,method)*durationWDaycount(today, T, "ACT/360"));
}


double OIS::fwd(const boost::gregorian::date &T1, const boost::gregorian::date &T2, const std::string &method)
{
    return ((1+zero(T2, method)*durationWDaycount(today, T2, "ACT/360"))/(1+zero(T1, method)*durationWDaycount(today, T1, "ACT/360")) - 1)/(durationWDaycount(T1, T2, "ACT/360"));
}

