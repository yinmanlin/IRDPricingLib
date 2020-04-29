#include "MathUtil.h"

double normalCDF(const double& value, const double& mu, const double& sigma)
{
    return (1.0 + erf( (value - mu) / sigma/sqrt(2.0))) / 2.0;
}


double normalPDF(const double& value, const double& mu, const double& sigma)
{
    return exp(-(value - mu) * (value - mu) / 2.0/sigma/sigma) / sigma / sqrt(2.0*M_PI);
}
