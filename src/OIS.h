#include <vector>
#include <string>
#include "IDiscount.h"
#include "Interpolation.h"

class OIS : public IDiscount
{
public:
    // read in data
    boost::gregorian::date today;
    std::vector<boost::gregorian::date> maturity;
    std::vector<std::string> dayCount;
    std::vector<int> payFreq;
    std::vector<double> swapRates;
    
    // desired output
    // elements are time, zeroRates and DF respectively
    std::vector<std::tuple<boost::gregorian::date, double, double>> curve;
    //std::vector<boost::gregorian::date> time;
    //std::vector<double> zeroRates;
    //std::vector<double> DF;
    //calculate fwdRate seperately
    std::vector<double> fwdRates;

    
    //initialize ZeroOIS with today and filename
    OIS(const boost::gregorian::date &, const std::string &);
    void Calibrate();
    double zero(const boost::gregorian::date &T, const std::string&);
    double DF(const boost::gregorian::date &T, const std::string &);
    double fwd(const boost::gregorian::date &T1, const boost::gregorian::date &T2,const std::string &);
};