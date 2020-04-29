#include "BlackModel.h"

BlackModel::BlackModel(boost::gregorian::date v,
                       boost::gregorian::date t1,
                       double discount,
                       double f,
                       double s,
                       double vd,
                       std::string t,
                       std::string m,
                       std::string dC)
{
    valuation = v;
    df = discount;
    fwd = f;
    tau = durationWDaycount(v, t1, dC);
    strike = s;
    vol = vd;
    type = t;
    method = m;
    dayCount = dC;
}


void BlackModel::setF(const double& a){fwd=a;}
void BlackModel::setDF(const double& a) {df = a;}
void BlackModel::setK(const double& a){strike=a;}
void BlackModel::setvol(const double& a){vol=a;}
void BlackModel::setPayDate(const boost::gregorian::date& t) {tau = durationWDaycount(valuation, t, dayCount);}
void BlackModel::setType(const std::string& t) {type = t;}

double BlackModel::OptionPrice()
{
   
    if (type == "call")
    {
        return  df * (fwd*normalCDF(d1()) - strike*normalCDF(d2()));
    }
    else
    {
        return  df * (strike*normalCDF(-d2()) - fwd*normalCDF(-d1()));
    }
    
}

double BlackModel::d1()
{
    return (log(fwd/strike)+(0.5*vol*vol)*tau)/(vol*sqrt(tau));
}

double BlackModel::d2()
{
    return d1() - vol * sqrt(tau);
}

double BlackModel::Vega()
{
    return df * (fwd * normalPDF(d1()) * (sqrt(tau)/2.0 - log(fwd/strike)/sqrt(tau)/vol/vol) + strike * normalPDF(d2())*(sqrt(tau)/2.0 + log(fwd/strike)/sqrt(tau)/vol/vol));
}

