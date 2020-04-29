#include "BaseContract.h"
#include "LIBOR.h"

class Swap : BaseContract {
public:
    //Input
    //std::string Name;
    boost::gregorian::date valuation_date; //libor
    boost::gregorian::date effective_date; //ois
    boost::gregorian::date T;  //SwapExpire
    double tenor; //LIBOR tenor
    double percent;  // spread over benchmark
    std::vector<boost::gregorian::date> PaymentDate;
    std::vector<boost::gregorian::date> ResetDate;
    std::string Method;//Interpolation
    
    //Constructor
    Swap();
    Swap(boost::gregorian::date v, boost::gregorian::date e,boost::gregorian::date t, double Tenor, double pct,std::string method);
    
    //Unique Setter
    void SetValuation_date(boost::gregorian::date t);
    void SetEffective_date(boost::gregorian::date OE);
    void SetT(boost::gregorian::date SE);
    void SetTenor(int t);
    void SetPercent(double pct);
    void SetMethod(std::string method);
    
    //Calculate PV and set FixedRate
    double FixedRate(OIS& , LIBOR&);
    //Derived
    std::string name() const { return "Swap"; }
    
};