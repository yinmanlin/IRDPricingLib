#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "LIBOR.h"


LIBOR::LIBOR(const boost::gregorian::date &t, const double &tenor, const std::string &filename1, OIS &ois) : IDiscount()
{
    today = t;
    std::ifstream w;
    w.open(filename1);
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
        getline(ss, substring, ',');
        if (!substring.empty() && substring[substring.size() - 1] == '\r')
            substring.erase(substring.size() - 1);
        source.push_back(substring);
        
    }
    // checked data read in are all correct
    Calibrate(t, tenor, ois);
}



void LIBOR::Calibrate(const boost::gregorian::date &t, const double &tenor, OIS &ois)
{
    
    // first build yield curve
    // iterate through every swap contract
    for (int i = 0; i <= maturity.size() - 1; ++i)
    {
       
        //std::cout << "M: " << maturity[i] << "\n";
        std::vector<boost::gregorian::date> payDates = paymentDates(today, maturity[i], payFreq[i]);
        
        // if there is only one cash flow exchange
        // payFreq = 0
        if(payDates.size() == 2)
        {
            if (source[i] == "FUTURE")
            {
                // convexity adjustment to be added
                // auto fwdr = cnvtAdjust(swapRates[i]);
                auto fwdr = swapRates[i];
                auto t = std::make_tuple(maturity[i], fwdr);
                curve.push_back(t);
            }
            // assume all other situations are cash
            else
            {
                // here calculates df using simple interest rate
                // zero rate calculation ???
                auto t = std::make_tuple(maturity[i], swapRates[i]);
                curve.push_back(t);
            }
        }else
        {
            // find forward rate on each payDate using swap quotes
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
                                       [&i](const std::tuple<boost::gregorian::date, double>& e){ return std::get<0>(e) == *i;} );
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
            double latestFwd = 0.0;
            // float leg sum
            double numerator1 = 0.0;
            // fix leg sum
            double numerator2 = 0.0;
            int rateIndex = 0;
            
            //对于reverse +是从后往前 -是从前往后
            // 从payDates的倒数第二个元素循环到第二个元素
            // implicitly assuming fixed leg using 30/360
            for (auto j = payDates.rbegin() + 1; j != payDates.rend() - 1; ++j )
            {
                // note here real cash flow exchange happens 3 months after
                //std::cout << "reset date: " << *j << "\n";
                boost::gregorian::date payD = returnWeekday(*j + boost::gregorian::months(tenor));
                double df = ois.DF(payD, "linear");
                //std::cout << "cf date: " << payD << " " << df << " " << ois.zero(payD, "linear") << "\n";
                // 这里例子是算3 month LIBOR 不需要calculate duration
                // 改一下加入day count
                // implicitly assuming fixed leg using 30/360
                // can be modified later
                numerator1 += tenor/12 * tempRate[rateIndex] * df;
                //std::cout << "fwd rate: " << tempRate[rateIndex] << "\n";
                numerator2 += tenor/12 * swapRates[i] * df;
                //std::cout << "swap rate: " << swapRates[i] << "\n";
                ++rateIndex;
            }
            boost::gregorian::date payD1 = returnWeekday(payDates[0] + boost::gregorian::months(tenor));
            //std::cout << "last reset date: " << payDates[0] << "\n";
            double denominator = ois.DF(payD1, "linear");
            //std::cout << "last cf date: " << payD1 << " " << denominator << " " << ois.zero(payD1, "linear") << "\n";
            numerator2 += tenor/12.0 * swapRates[i] * denominator;
            //std::cout << "numerator2: " << numerator2 << "\n";
            latestFwd = (numerator2 - numerator1)/denominator;
            //std::cout << "numerator1: " << numerator1 << "\n";
            auto t = std::make_tuple(maturity[i], latestFwd);
            curve.push_back(t);
        }
    }
    
}


double LIBOR::fwd(const boost::gregorian::date &T, const std::string &method)
{
    return interpolation(T, curve, method);
}

double LIBOR::DF(const boost::gregorian::date &T, const std::string &method)
{
    return 1.0;
}
