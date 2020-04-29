#include "Swap.h"

//Constructor
Swap::Swap(){
    //Name = "null";
    valuation_date = boost::gregorian::day_clock::universal_day();
    effective_date = boost::gregorian::day_clock::universal_day();
    T = boost::gregorian::day_clock::universal_day();
    tenor = 3;
    percent = 0.0;
    PaymentDate = std::vector<boost::gregorian::date>();
    ResetDate = std::vector<boost::gregorian::date>();
}

Swap::Swap(boost::gregorian::date v, boost::gregorian::date e, boost::gregorian::date t, double Tenor, double pct, std::string method) {
    //General member
    valuation_date = v;
    effective_date = e;
    T = t;
    percent = pct;
    tenor = Tenor;
    Method = method;
    //Set ResetDate for LIBOR rates
    // effective date is first reset date check!!!!!!!
    ResetDate = paymentDates(effective_date, T, tenor);
    PaymentDate = ResetDate;
    ResetDate.erase(ResetDate.end() - 1);
    PaymentDate.erase(PaymentDate.begin());
}

//Unique Setter
void Swap::SetValuation_date(boost::gregorian::date v) {
    valuation_date = v;
}
void Swap::SetEffective_date(boost::gregorian::date e) {
    effective_date = e;
}
void Swap::SetT(boost::gregorian::date t) {
    T = t;
}
void Swap::SetTenor(int t) { tenor = t; }
void Swap::SetPercent(double pct) { percent = pct; }
void Swap::SetMethod(std::string method) { Method = method; }

//Calculate fixed rate
double Swap::FixedRate(OIS& Ois, LIBOR& Libor)
{
    //Calculate Floating cash flow
    double Float = 0.0;
    double fwdRate = 0.0;
    double df = 0.0;
    double fixedDiscount = 0.0;
    for (size_t i = 0; i < PaymentDate.size();++i)
    {
        //Find LIBOR Forward Rate--------------------------------------------------------------
        fwdRate = Libor.fwd(ResetDate[i], Method)+percent;
        //Find OIS rate
        df = Ois.DF(PaymentDate[i], Method);
        //Calculate Floating Cash Flow for date t
        Float += (fwdRate * tenor / 12.0) * df;
        fixedDiscount += df;
    }
    return Float / fixedDiscount / (tenor/12.0);
}

