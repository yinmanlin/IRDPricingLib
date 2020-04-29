#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include "DateUtil.h"
#include "LIBOR.h"
#include "BaseContract.h"
#include "BlackModel.h"
// using namespace std;

class CapFloor: public BaseContract{
public:
    // *all rates are in annual basis
    boost::gregorian::date Valuation_date;      // date that the cap/floor and all caplets/floorlets will be priced at.
    boost::gregorian::date Effective_date;      // date that the first payment occurs
    boost::gregorian::date Maturity;                   // contract maturity
    double Strike_Rate;       // cap rate or floor rate
    double Volatility;        // volatility of forward interest rate
    double Frequency;            // in months. usually 3 months, 0.25 year
    std::string InterpolationMethod;
    //int Cap_or_Floor;       // cap == 0; floor == 1;
    std::string Cap_or_Floor;      // Product Type
    std::string day_count_basis;   // date count convention
    
    // CapFloor() {}
    CapFloor(boost::gregorian::date valuation,
             boost::gregorian::date effective,
             boost::gregorian::date Maturity,
             double strike,
             double vol,
             double freq,
             std::string cap_floor,
             std::string basis,
             std::string method);
    CapFloor(std::string cap_floor): Cap_or_Floor{cap_floor} {}
    
    void SetCapFloor(boost::gregorian::date valuation, boost::gregorian::date effective, boost::gregorian::date maturity, double strike, double vol, double freq, std::string cap_floor, std::string basis, std::string method);
    
    void SetValuationDate(const boost::gregorian::date& valuation);
    void SetEffectiveDate(const boost::gregorian::date& effective);
    void SetMaturity(const boost::gregorian::date& maturity);
    void SetStrikeRate(const double& strike);
    void SetVolatility(const double& vol);
    void SetFrequency(const double& freq);
    void SetCapFloor(const std::string& cap_floor);
    void SetBasis(const std::string& basis);
    void SetInterpolationMethod(const std::string& method);
    
    // 继承自Base
    std::string name() const {return "Cap or Floor";}
    
    //price at each caplet/floorlet and sum up
    double Price(OIS&, LIBOR&);
};