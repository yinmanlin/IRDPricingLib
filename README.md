# IRDPricingLib
This is a project aims to provide pricing functionalities for various fixed income products, including swaps, caps/floors, swaptions and exotic products.

# How to use:
Step 1: Yield Curve Construction. <br>
Initialize current OIS curve and LIBOR forward curve with market data residing in data folder.

boost::gregorian::date today{2020,4,29}; <br>
OIS ois(today, "./data/OIS.csv"); <br>
LIBOR l(today, 3, "./data/LIBOR.csv", ois); <br>
 
Step 2: Initialize interest rate derivative contracts and call pricing methods. <br>

Swap contract: <br>
boost::gregorian::date d1{2023,4,29}; <br>
Swap s(today, today, d1, 3, 0, "linear"); <br>
std::cout << "Swap price: " << s.FixedRate(ois, l) << "\n";

Cap contract: <br>
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
