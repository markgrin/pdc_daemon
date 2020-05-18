#include "bin_poi.hpp"

#include <boost/multiprecision/cpp_complex.hpp>
#include <complex>


namespace pdc {

namespace {

std::complex<long double> mul(long double left, std::complex<long double> right) {
    return std::complex<long double>(left * right.real(), left * right.imag());
}

std::complex<long double> plus(long double left, std::complex<long double> right) {
    return std::complex<long double>(left + right.real(), right.imag());
}

std::complex<long double> minus(long double left, std::complex<long double> right) {
    return std::complex<long double>(right.real() - left, right.imag());
}

} // namespace

std::vector<std::complex<long double>> bin_poi::construct_pmfs() {
    std::vector<std::complex<long double>> chi(success_probabilities.size() + 1);
    std::size_t half_size = success_probabilities.size() / 2 + (success_probabilities.size() % 2);
    for (std::size_t i = 0; i <= half_size; i++) {
        chi[i] = exp(i * omega * std::complex<long double>(0, 1));
    }
    for (std::size_t x = 0; x < half_size; x++) {
        long double argz_sum = 0;
        long double exparg = 0;
        for (std::size_t y = 0; y < success_probabilities.size(); y++) {
            auto temp = plus(1, mul(success_probabilities[y], chi[x + 1]));
            temp = minus(success_probabilities[y], temp);
            argz_sum += atan2(temp.imag(), temp.real());
            exparg += log(abs(temp));
        }
        auto d_value = exp(exparg);
        chi[x + 1] = d_value * exp(argz_sum * std::complex<long double>(0, 1));
    }
    chi[0] = 1;
    return chi;
}

bin_poi::bin_poi(std::vector<double> success_probabilities_in) :
        success_probabilities (std::move(success_probabilities_in)),
        omega (0) {
    for (auto p : success_probabilities) {
        if (p > 1 || p < 0) {
            throw std::runtime_error("probability should be bounded in [0, 1]");
        }
    }
    // 2 * PI / (n + 1)
    omega = 2 * (3.1415926535897932384626433832795) / (success_probabilities.size() + 1);
    auto chis = construct_pmfs();
    for (std::size_t i = 0; i < success_probabilities.size() / 2; i++) {
        const auto& conj = chis[1 + i];
        chis[chis.size() - 1 - i] = std::complex<long double>(conj.real(), conj.imag() * (-1));
    }
    for (auto& chi : chis) {
        chi /= success_probabilities.size() + 1;
    }
    auto sum = chis[0];
    for (std::size_t i = 1; i < chis.size(); i++)
       sum += chis[i];
    std::vector<std::complex<long double>> xis(success_probabilities.size() + 1);
    auto pi = boost::math::constants::pi<long double>();
    auto e = boost::math::constants::e<long double>();
    auto i = std::complex<long double>(0, 1);
    std::complex<long double> second(0, 0);
    for (std::size_t c = 0; c < chis.size(); c++) {
        xis[c] = 0;
        for (std::size_t n = 0; n < chis.size(); n++) {
            xis[c] += chis[n] * pow(e, (mul((-2), i) * pi * mul(c, n)) / std::complex<long double>(chis.size(), 0));
        }
    }
    for (auto xi : xis) {
        pmfs.push_back(xi.real());
    }
    cdfs.push_back(pmfs.front());
    for (std::size_t i = 1; i < pmfs.size(); i++) {
        cdfs.push_back(cdfs[i - 1] + pmfs[i]);
    }
}

int bin_poi::more_than(double probability_level) {
    for (std::size_t i = 0; i < cdfs.size(); i++) {
        if (1 - cdfs[i] < probability_level) {
            return i == 0 ? 0 : (i + cdfs.size() - 1) % cdfs.size();
        }
    }
    return 0;
}

int bin_poi::leq_than(double probability_level) {
    for (std::size_t i = 0; i < cdfs.size(); i++) {
        if (cdfs[i] > probability_level) {
            return i;
        }
    }
    return cdfs.size();
}

} // namespace pdc
