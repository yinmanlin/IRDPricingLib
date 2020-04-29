#include "MathUtil.h"
#include "DateUtil.h"
#include "LIBOR.h"


class BlackModel{ //here changed name of class
public:
    double fwd, df, tau, strike, vol;
    std::string type, method, dayCount;
    boost::gregorian::date valuation;

    //std::string name(){return "Black_model";}
    
    BlackModel(boost::gregorian::date,
               boost::gregorian::date,
               double,
               double,
               double,
               double,
               std::string,
               std::string,
               std::string);
    
    //setter
    void setF(const double& a);
    void setDF(const double& a);
    void setK(const double& a);
    void setvol(const double& a);
    void setPayDate(const boost::gregorian::date& t);
    void setType(const std::string& t);
    
    double d1();
    double d2();
    double OptionPrice();
    double Vega();
};