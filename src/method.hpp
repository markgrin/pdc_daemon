#ifndef HPP_7E2AD4CECAA24FE6941C2951D98BFCDA

#define HPP_7E2AD4CECAA24FE6941C2951D98BFCDA

#include "call.hpp"
#include <vector>
#include <variant>

namespace pdc {

struct MethodResult {
    bool is_rate = false;
    double rate = 0.f;
    std::size_t calls;

    explicit MethodResult (std::size_t calls_in)
    :
    is_rate (false),
    rate (0.f),
    calls(calls_in) {}

    explicit MethodResult (double rate_in)
    :
    is_rate(true),
    rate (rate_in),
    calls(0) {}

};

struct Method {
    virtual void addCall(Call &call) = 0;
    virtual void addBusy(double setup) = 0;

    virtual MethodResult calculate(std::size_t agents,
                                    std::vector<double> setup,
                                    std::vector<double> service) = 0;
    virtual ~Method(){}
};

} // namespace pdc

#endif // HPP_7E2AD4CECAA24FE6941C2951D98BFCDA
