#include "Interpolation.h"

double interpolation(const boost::gregorian::date &dt, std::vector<std::tuple<boost::gregorian::date, double, double>> &curve, const std::string &method)
{
    // first sort curve using date, first tuple element
    sort(curve.begin(), curve.end());
    
    //std::cout << "curve last date: \n" << std::get<0>(curve[curve.size()-1]) << "\n";
    //std::cout << "curve last date: \n" << std::get<0>(*(curve.end())) << "\n";
    // identify two closest dates and corresponding rates
    auto itr = std::lower_bound(curve.begin(), curve.end(), std::make_tuple(dt,1.0,1.0),
                                [](std::tuple<boost::gregorian::date, double, double> lhs, std::tuple<boost::gregorian::date, double, double> rhs) -> bool { return std::get<0>(lhs) < std::get<0>(rhs); });
    if (method == "linear")
    {
        if (itr == curve.end())
        {
            // 这里如果超过了期限 return了最近的利率 并没有写extrapolation
            // 可以改进
            return std::get<1>(curve[curve.size()-1]);
        }else if(itr == curve.begin())
        {
            return std::get<1>(curve[0]);
        }else
        {
            auto itl = itr - 1;
            // right date
            auto dt2 = std::get<0>(*itr);
            //std::cout << "right date: " << dt2 << "\n";
            // left date
            auto dt1 = std::get<0>(*itl);
            //std::cout << "left date: " << dt1 << "\n";
            // right zero rate
            auto r2 = std::get<1>(*itr);
            //std::cout << "right rate: " << r2 << "\n";
            // left zero rate
            auto r1 = std::get<1>(*itl);
            //std::cout << "left rate: " << r1 << "\n";
            double t1 = (dt - dt1).days();
            double t2 = (dt2 - dt1).days();
            return r1 + t1/t2 * (r2-r1);
        }
    }else
    {
        // to be implemented
        return 0.0;
    }
}

double interpolation(const boost::gregorian::date &dt, std::vector<std::tuple<boost::gregorian::date, double>> &curve, const std::string &method)
{
    // first sort curve using date, first tuple element
    sort(curve.begin(), curve.end());
    
    //std::cout << "curve last date: \n" << std::get<0>(curve[curve.size()-1]) << "\n";
    //std::cout << "curve last date: \n" << std::get<0>(*(curve.end())) << "\n";
    // identify two closest dates and corresponding rates
    auto itr = std::lower_bound(curve.begin(), curve.end(), std::make_tuple(dt,1.0),
                                [](std::tuple<boost::gregorian::date, double> lhs, std::tuple<boost::gregorian::date, double> rhs) -> bool { return std::get<0>(lhs) < std::get<0>(rhs); });
    if (method == "linear")
    {
        if (itr == curve.end())
        {
            // 这里如果超过了期限 return了最近的利率 并没有写extrapolation
            // 可以改进
            return std::get<1>(curve[curve.size()-1]);
        }else if(itr == curve.begin())
        {
            std::cout << "First out of range \n";
            return std::get<1>(curve[0]);
        }else
        {
            auto itl = itr - 1;
            // right date
            auto dt2 = std::get<0>(*itr);
            //std::cout << "right date: " << dt2 << "\n";
            // left date
            auto dt1 = std::get<0>(*itl);
            //std::cout << "left date: " << dt1 << "\n";
            // right zero rate
            auto r2 = std::get<1>(*itr);
            //std::cout << "right rate: " << r2 << "\n";
            // left zero rate
            auto r1 = std::get<1>(*itl);
            //std::cout << "left rate: " << r1 << "\n";
            double t1 = (dt - dt1).days();
            double t2 = (dt2 - dt1).days();
            return r1 + t1/t2 * (r2-r1);
        }
    }else
    {
        // to be implemented
        return 0.0;
    }
}
