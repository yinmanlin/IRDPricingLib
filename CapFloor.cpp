#include <iostream>
#include "CapFloor.h"

CapFloor::CapFloor(boost::gregorian::date valuation, boost::gregorian::date effective, boost::gregorian::date maturity, double strike, double vol, double freq, std::string cap_floor, std::string basis, std::string method){
    Valuation_date = valuation;
    Effective_date = effective;
    Maturity = maturity;
    Strike_Rate = strike;
    Volatility = vol;
    Frequency = freq;
    Cap_or_Floor = cap_floor;
    day_count_basis = basis;
    InterpolationMethod = method;
    
}

void CapFloor::SetCapFloor(boost::gregorian::date valuation, boost::gregorian::date effective, boost::gregorian::date maturity, double strike, double vol, double freq, std::string cap_floor, std::string basis, std::string method){
    Valuation_date = valuation;
    Effective_date = effective;
    Maturity = maturity;
    Strike_Rate = strike;
    Volatility = vol;
    Frequency = freq;
    Cap_or_Floor = cap_floor;
    day_count_basis = basis;
    InterpolationMethod = method;
}

void CapFloor::SetValuationDate(const boost::gregorian::date& valuation) {Valuation_date = valuation;}
void CapFloor::SetEffectiveDate(const boost::gregorian::date& effective) {Effective_date = effective;}
void CapFloor::SetMaturity(const boost::gregorian::date& maturity)  {Maturity = maturity;}
void CapFloor::SetStrikeRate(const double& strike) {Strike_Rate = strike;}
void CapFloor::SetVolatility(const double& vol) {Volatility = vol;}
void CapFloor::SetFrequency(const double& freq) {Frequency = freq;}
void CapFloor::SetCapFloor(const std::string& cap_floor) {Cap_or_Floor = cap_floor;}
void CapFloor::SetBasis(const std::string& basis) {day_count_basis = basis;}
void CapFloor::SetInterpolationMethod(const std::string& method) {InterpolationMethod = method;}

//double CapFloor::phi(double x){
//    return (1.0 + erf(x/sqrt(2.0))) / 2.0;
//}

double CapFloor::Price(OIS& zero_obj, LIBOR& fwd_obj){
    double value = 0.0;           // value of each caplet or floorlet
    
    // Payment Dates
    std::vector<boost::gregorian::date> Reset_dates = paymentDates(Effective_date, Maturity, Frequency);
    std::vector<boost::gregorian::date> Payment_dates = Reset_dates;
    Reset_dates.erase(Reset_dates.end() - 1);
    Payment_dates.erase(Payment_dates.begin());
    
    // temporary object, parameter will be modified afterwards
    BlackModel bm(Valuation_date, Valuation_date, Volatility, Volatility, Strike_Rate, Volatility, InterpolationMethod, InterpolationMethod, day_count_basis);
    
    // calc each cashflow and sum up
    // 理想情况下， reset和payment的数量应该是一致的。这里先暂时加一个if试一下。
    if(Payment_dates.size() == Reset_dates.size()){
        for(int i = 0; i < Payment_dates.size(); ++i){
            double df = zero_obj.DF(Payment_dates[i], InterpolationMethod);
            double fwd = fwd_obj.fwd(Reset_dates[i], InterpolationMethod);
            bm.setPayDate(Reset_dates[i]);
            bm.setF(fwd);
            bm.setDF(df);
            // calculate each caplet value
            if(Cap_or_Floor == "Cap"){
                bm.setType("call");
                value += Frequency/12.0*bm.OptionPrice();
            }
            // calculate each floorlet value
            else{
                bm.setType("put");
                value += Frequency/12.0*bm.OptionPrice();
            }
        }
        return value;
    }
    else
        throw "The number of total reset dates and payment dates doesn't match, please check again! ";
}
