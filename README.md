# IRDPricingLib
This is a project aims to provide pricing functionalities for various fixed income products, including swaps, caps/floors, swaptions and exotic products.

# How to use:
Step 1: Yield Curve Construction. <br>
Initialize current OIS curve and LIBOR forward curve with market data residing in data folder.

boost::gregorian::date today{2020,4,29}; <br>
OIS ois(today, "./data/OIS.csv"); <br>
LIBOR l(today, 3, "./data/LIBOR.csv", ois); <br>
 
