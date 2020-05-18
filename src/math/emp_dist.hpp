#ifndef HPP_A5DD1A1091FC4581AA989BB007BE8170

#define HPP_A5DD1A1091FC4581AA989BB007BE8170

#include <deque>
#include <map>

class EmpDist {
    std::deque<double> points;
    std::map<double, std::size_t> sorted;
    std::size_t max;
public:
    EmpDist () = delete;
    EmpDist (std::size_t maxLength) : max (maxLength) {}

    /**
     * Add value to empirical distribution.
     */
    void add (double x);

    /**
     * Cummulative density function
     */
    double cdf (double x) const;

    /**
     * p( x < a + b | x > a ) = p ( a < x < a + b ) / p ( x > a )
     */
    double cond (double a, double b) const;

    double mean () const;

    bool is_full() const { return points.size() == max; }

};

#endif // HPP_A5DD1A1091FC4581AA989BB007BE8170