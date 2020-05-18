#ifndef HPP_1EBD42EF3B3C42199B6BE5867904D716

#define HPP_1EBD42EF3B3C42199B6BE5867904D716

#include "method.hpp"
#include "math/emp_dist.hpp"
#include <iostream>

namespace pdc {

class Progressive : public Method {
    public:
    void addCall(Call& call) override {
        static EmpDist statistics(200);
        statistics.add(call.service);
    }
    void addBusy(double setup) override {
        return ;
    }
    MethodResult calculate (std::size_t agents,
                           std::vector<double> setup,
                           std::vector<double> service) override
    {
        return MethodResult(agents > setup.size() + service.size() ? agents - setup.size() - service.size() : 0);
    }
};


} // namespace pdc

#endif /* HPP_1EBD42EF3B3C42199B6BE5867904D716 */
