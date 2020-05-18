#include "berlang.hpp"

#include <cmath>

namespace pdc {

double berlang (double intensity, std::size_t agents) {
    if (!intensity || !agents) {
        return 0.f;
    }
    double inverted_b = 1;
    for (std::size_t i = 1; i <= agents; i++) {
        inverted_b = 1.0 + inverted_b * static_cast<double>(i) / intensity; 
    }
    return (inverted_b > 0 ? static_cast<double>(1) / inverted_b : 0.f);
}

double iberlang (double probability, std::size_t agents) {
    std::size_t cycles = 64;
    double min = 0.f;
    double max = 1024 * 1024;
    for (std::size_t i = 0; i < cycles; i++) {
        double middle = (min + max) / 2;
        double current = berlang(middle, agents);
        if (current < probability) {
            min = middle;
        } else {
            max = middle;
        }
    }
    return (min + max) / 2;
}

} // namespace pdc