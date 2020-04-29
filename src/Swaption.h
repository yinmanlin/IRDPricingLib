#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <boost/date_time.hpp>
#include "LIBOR.h"
#include "DateUtil.h"
#include "Swap.h"
#include "BlackModel.h"

class Swaption: public Swap{
public:
    
    
    //General-------------------------------------------------
    //boost::gregorian::date valuation_date; //Today
    //boost::gregorian::date effective_date; //OptionExpire
    //boost::gregorian::date T; //Swap Expire
    //double principal;
    //double frequency;
    
    //Unique---------------------------------------------------
    double SwapRate; //St
    double StrikeRate; // Sk
    double Vol; //Volatility of Foward Swap Rate
    std::string dayCount;
    std::string SwaptionType;

    
    //Constructor
    Swaption();
    Swaption(boost::gregorian::date v,
                  boost::gregorian::date e,
                  boost::gregorian::date t,
                  double Tenor,
                  double pct,
                  std::string method,
                  double k,
                  double vol,
                  std::string day_count_basis,
                  std::string type);
    
    //Setters
    //General(to be edited)-------------------------------------------------------
    void SetValuation_date(boost::gregorian::date t);
    void SetEffective_date(boost::gregorian::date e);
    void SetT(boost::gregorian::date t);
    
    //Unique------------------------------------------------------------------------
    void SetStrikeRate(double k); // Sk
    void SetVol(double vol); //Volatility of Foward Swap Rate
    void SetDayCount(std::string dayCountBASIS);
    
    //Derived Override
    std::string name() const { return "Swaption"; }
    double Price(OIS& Ois, LIBOR& Libor);
};

