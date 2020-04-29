#include "Swaption.h"

Swaption::Swaption() {
    valuation_date = boost::gregorian::day_clock::universal_day();
    effective_date = boost::gregorian::day_clock::universal_day();
    T = boost::gregorian::day_clock::universal_day();
    SwapRate = 0.0;
    StrikeRate = 0.0;
    Vol = 0.0;
    dayCount = "ACT/365";
    tenor = 3.0;
    Method = "linear";
    SwaptionType = "call";
}

Swaption::Swaption(boost::gregorian::date v,
                             boost::gregorian::date e,
                             boost::gregorian::date t,
                             double Tenor,
                             double pct,
                             std::string method,
                             double k,
                             double vol,
                             std::string day_count_basis,
                             std::string type) : Swap(v, e, t, Tenor, pct, method)
{
    
    StrikeRate = k;
    SwaptionType = type;
    Vol = vol;
    dayCount = day_count_basis;
    
}

//Setters
//Oeverride---------------------------------------------------------------
void Swaption::SetValuation_date(boost::gregorian::date v) {
    valuation_date = v;
}
void Swaption::SetEffective_date(boost::gregorian::date e) {
    effective_date = e;
}
void Swaption::SetT(boost::gregorian::date t) {
    T = t;
}

//Unique---------------------------------------------------------------------------------
void Swaption::SetStrikeRate(double k){ StrikeRate = k; }
void Swaption::SetVol(double vol) { Vol = vol; }
void Swaption::SetDayCount(std::string dayCountBASIS) {
    dayCount = dayCountBASIS;
}

double Swaption::Price(OIS& Ois, LIBOR& Libor)
{
    double fwdSwapRate = FixedRate(Ois, Libor);
    double price = 0.0;
    double discount = Ois.DF(effective_date, Method);
    
    BlackModel bm(valuation_date, effective_date, discount, fwdSwapRate, StrikeRate, Vol, SwaptionType, Method, dayCount);
    bm.setDF(discount);
    double optionPrice = bm.OptionPrice();
    for (size_t i = 0; i < PaymentDate.size();++i)
    {
        double df = Ois.DF(PaymentDate[i], Method);
        price += tenor/12.0*df*optionPrice;
    }
    return price;
}

