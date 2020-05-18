#ifndef HPP_B8FC6211CE06414788CB151FBE2FC2E4

#define HPP_B8FC6211CE06414788CB151FBE2FC2E4

#include "method.hpp"
#include "math/emp_dist.hpp"
#include "math/berlang.hpp"
#include "math/bern_stat.hpp"
#include "progressive.hpp"


#include <iostream>

namespace pdc {

class Berlang : public Method {
    EmpDist setup_dist = EmpDist(200);
    EmpDist service_dist = EmpDist(200);
    BernStat busy_stat = BernStat(200);
    double critical = 0.03;

public:
    void addCall(Call& call) override {
        setup_dist.add(call.setup);
        service_dist.add(call.service);
        busy_stat.add(true);
    }
    void addBusy(double add) override {
        busy_stat.add(false);
    }
    void set_critical(double critical_in) { critical = critical_in; }
    MethodResult calculate (std::size_t agents,
                           std::vector<double> setup,
                           std::vector<double> service) override {
        if (!setup_dist.is_full() || !service_dist.is_full()) {
            return Progressive().calculate(agents, setup, service);
        }
        double possible_intensity = iberlang(critical, agents);
        possible_intensity; // /= pickup prob.
        double mean = service_dist.mean();
        double per_second = (mean ? possible_intensity / (mean * busy_stat.get_p()) : 0);
        return MethodResult(per_second);
    }
};

} // namespace pdc

#endif // HPP_B8FC6211CE06414788CB151FBE2FC2E4