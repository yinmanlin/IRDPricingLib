#include <iostream>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "MathUtil.h"

int main()
{
    double a = 2.0;
    std::cout << normalCDF(a) << "\n";
    return 0;
}