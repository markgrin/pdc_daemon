#include "bern_stat.hpp"

namespace pdc {

void BernStat::add (bool success) {
    data.push_back(success);
    while (data.size() > max) {
        data.pop_front();
    }
    cache.reset();
}

double BernStat::get_p () const {
    if (data.empty()) {
        return 0u;
    }
    if (cache) {
        return cache.value();
    }
    std::size_t success = 0u;
    for (auto item : data) {
        if (item) {
            success += 1;
        }
    }
    double result = static_cast<double>(success) / static_cast<double>(data.size());
    cache = result;
    return result;
}

double BernStat::get_binomial(std::size_t n, double prob_limit) const {
    double p = get_p();
    double binomial = n;
    double acc_p = 1;
    double all_p = 1;
    
    for (std::size_t i = 0; i < n + 1; i++) {
        if (i == 0) {
            for (std::size_t j = 0; j < n; j++) {
                acc_p *= (1 - p);
            }
            all_p = acc_p;
            binomial = 1;
            if (all_p > prob_limit) {
                return i;
            }
            continue;
        }
        if (i == 1) {
            binomial = n;
        } else { 
            binomial *= (n - (i - 1));
            binomial /= (i);
        }
        acc_p *= p;
        acc_p /= (1 - p);
        all_p += acc_p * binomial;
        if (all_p > prob_limit) {
            return i;
        }
    }
    return n;
}

} // namespace pdc