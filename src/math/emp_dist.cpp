#include "emp_dist.hpp"

#include <stdexcept>
#include <iostream>


void EmpDist::add (double x) {
    points.push_front(x);
    if (points.size() > max) {
        points.pop_back();
    }
}

double EmpDist::cdf (double x) const {
    std::size_t sum = 0;
    for (auto point : points)
        if (point < x)
            sum++;
    if (std::empty(points))
        return 0;
    return static_cast<double>(sum) / std::size(points);
}

double EmpDist::cond (double a, double b) const {
    if (std::empty(points))
        return 1;
    std::size_t passed = 0;
    for (auto point : points)
        if (a + b > point && point > a)
            passed++;

    double numerator = static_cast<double>(passed) / std::size(points);

    double denominator = (1 - cdf(a));

    return numerator / denominator;
}

double EmpDist::mean () const {
    if (std::empty(points)) {
        return 0;
    }
    double sum = 0;
    for (auto point : points) {
        sum += point;
    }
    return sum / points.size();
}