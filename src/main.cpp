#include <iostream>
#include <fstream>
#include <vector>
#include "LIBOR.h"
#include "CapFloor.h"
#include "Swaption.h"


int main() {

    // construct yield curve
    boost::gregorian::date today{2020,4,29};
    boost::gregorian::date d1{2023,4,29};
    OIS ois(today, "OIS.csv");
    LIBOR l(today, 3, "LIBOR.csv", ois);

    // output ois rate
    std::cout << "OIS zero rate:\n";
    for (auto & e : ois.curve)
    {
        std::cout << std::get<0>(e) << "  " << std::get<1>(e) << "\n";
    }
    std::cout << "\n";
    
     // output libor rate
    std::cout << "3-month LIBOR rate:\n";
    for (auto & e : l.curve)
    {
        std::cout << std::get<0>(e) << "  " << std::get<1>(e) << "\n";
    }
    std::cout << "\n";
    
    // initialize swap contract
    Swap s(today, today, d1, 3, 0, "linear");
    std::cout << "Swap price: " << s.FixedRate(ois, l) << "\n";
    std::cout << "\n";
    
    // initialize cap contract
    boost::gregorian::date effective_date(2020, 07, 30);
    boost::gregorian::date Maturity(2025, 04, 30);
    double StrikeRate = 0.001;
    double vol = 0.3;
    double freq = 3;
    std::string cap_or_floor = "Cap";
    std::string swaption_type = "call";
    std::string Day_count_basis = "ACT/360";
    std::string Interpolate = "linear";
    CapFloor Cap1(today, effective_date, Maturity, StrikeRate, vol, freq, cap_or_floor, Day_count_basis, Interpolate);
    std::cout << "Cap price: " << Cap1.Price(ois, l) << "\n";
    
    
    /*
    double StrikeRate = 0.002;
    double vol = 0.4;
    double freq = 3;
    std::string cap_or_floor = "Cap";
    std::string basis = "ACT/360";
    
    boost::gregorian::date valuation_date(2020, 4, 19);
    boost::gregorian::date effective_date(2021, 4, 19);
    boost::gregorian::date Maturity(2025, 4, 19);
    
    
    std::string type ="call";
    std::string method ="linear";
    

    std::cout << "zero \n";
    for (auto & e : ois.curve)
    {
        std::cout << std::get<2>(e) << " ";
    }
    
    std::cout << "\n";
    
    
    for (auto & e : l.curve)
    {
        std::cout << std::get<0>(e) << " " <<  std::get<1>(e) << " ";
    }
    
    std:: cout << "\n";
    
    
    Swap s(today, effective_date, Maturity, freq, 0.0 , "linear");
    std:: cout << s.FixedRate(ois, l)<< "\n";
    
    Euro_Swaption so(today,
                  effective_date,
                  Maturity,
                  freq,
                  0.0,
                  "linear",
                  StrikeRate,
                  vol,
                  basis,
                  "put");
   
    std:: cout << so.Price(ois, l)<< "\n";
    */

   // 1 libor curve not accurate need convexity adjustment
   // 2 libor curve not stable              *done problem is with PaymentDate function
   // 3 european swaption price not okay    *done
   // 4 black implied vol
   // 5 check pricing is correct or not
   
   // 整体定价思路 structure
   // yield curve, ois comparison
   // black implied vol SABR vol surface cube
   // swap/tion
   // cap
   // further improvement SABR(vol) LMM(interest model simulate for pricing exotic products) Portfolio, record holding,
   // risk management system calculate portoflio greeks VaR
  
    return 0;
}
