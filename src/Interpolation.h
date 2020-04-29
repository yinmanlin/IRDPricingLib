#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include "DateUtil.h"


double interpolation(const boost::gregorian::date &dt, std::vector<std::tuple<boost::gregorian::date, double, double>> &curve, const std::string &method);



double interpolation(const boost::gregorian::date &dt, std::vector<std::tuple<boost::gregorian::date, double>> &curve, const std::string &method);

