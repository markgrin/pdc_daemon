#ifndef HPP_1C12F36067C74816881A6D6CBB7FD8C7

#define HPP_1C12F36067C74816881A6D6CBB7FD8C7

#include <deque>
#include <optional>

namespace pdc {


class BernStat {

    std::size_t max = 200;
    std::deque<bool> data;
    mutable std::optional<double> cache;

    public:

    explicit BernStat (std::size_t max_in)
    :
    max (max_in) {
    }

    void add (bool success);

    double get_p () const;

    double get_binomial(std::size_t n, double prob_limit) const;
};

} // namespace pdc


#endif // HPP_1C12F36067C74816881A6D6CBB7FD8C7