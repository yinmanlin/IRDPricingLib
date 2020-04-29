# IRDPricingLib
This is a project aims to provide pricing functionalities for various fixed income products, including swaps, caps/floors, swaptions and exotic products.

# How to use:
Step 1: Yield Curve Construction.
Construct current OIS curve and LIBOR forward curve.

OIS is a yield curve class, it reads data from a csv file containing OIS swap market data. Initialize the class with today's date (in format of boost::gregorian::date) and the csv filename. Csv file fields are Bid, Ask, Daycount, Freq, Maturity Date.

OIS has 3 methods:

zero takes in a date T (in format of boost::gregorian::date) and a string indicating the desired interpolation method, returns the zero rate of period from today to T.

DF works the same way, it calculates discount factor of corresponding date, using simple interest rate calculation(i.e. 1/(1+r*timeperiod)).

fwd takes in two dates T1 and T2 (in format of boost::gregorian::date) and a string indicating the desired interpolation method, returns the forward rate of period from T1 to T2.
