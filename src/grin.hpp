#ifndef HPP_3BE77A598E70458ABAF0AA33090ADD60

#define HPP_3BE77A598E70458ABAF0AA33090ADD60

#include "method.hpp"
#include "progressive.hpp"
#include "../math/emp_dist.hpp"
#include "../math/bin_poi.hpp"
#include "../math/bern_stat.hpp"
#include "../common/timer.hpp"
#include <iostream>

namespace pdc {

class Grin : public Method {
    EmpDist setup_dist = EmpDist(200);
    EmpDist service_dist = EmpDist(200);
    BernStat busy_dist = BernStat(200);
    double critical = 0.97;
    std::size_t no_skip = 0;

public:
    void addCall(Call& call) override {
        setup_dist.add(call.setup);
        service_dist.add(call.service);
        busy_dist.add(true);
    }
    void addBusy(double add) override {
        busy_dist.add(false);
       // service_dist.add(0);
        setup_dist.add(add);
    }
    void set_critical(double critical_in) {
        critical = critical_in;
    }
    MethodResult calculate (std::size_t agents,
                           std::vector<double> setup,
                           std::vector<double> service) override {
        if (!setup_dist.is_full() || !service_dist.is_full()) {
            return Progressive().calculate(agents, setup, service);
        }
        agents = agents - service.size();
        std::cout << agents << "\n";
        bool success = true;
        double mean_sum = (setup_dist.mean() + service_dist.mean()) / 3;
        double time_step = mean_sum / 3;
        if (no_skip) {
            no_skip--;
            return MethodResult(0ul);
        }
        for (double time = time_step; time < mean_sum ; time += time_step) {
            std::vector<double> pluses;
            std::vector<double> minuses;
            for (std::size_t i = 0; i < setup.size(); i++) {
                double value = setup_dist.cond(setup[i], time);
                if (value < 0 && value > -0.01) {
                    value = 0;
                } else if (value > 1 && value < 1.01) {
                    value = 1;
                }
                minuses.push_back(value);
            }
            for (std::size_t i = 0; i < service.size(); i++) {
                double value = service_dist.cond(service[i], time);
                if (value < 0 && value > -0.01) {
                    value = 0;
                } else if (value > 1 && value < 1.01) {
                    value = 1;
                }
                pluses.push_back(value);
            }
            bin_poi plus_poi(std::move(pluses));
            bin_poi minus_poi(std::move(minuses));
            int plus = plus_poi.more_than(critical);
            int minus = minus_poi.leq_than(critical);
            minus = busy_dist.get_binomial(minus, critical);
            int sum_for_time = plus - minus + static_cast<int>(agents);
            //std::cout << "AGENTS:" << agents << " sum:" << sum_for_time << " plus:" << plus << " minus:" << minus << "\n";
            if (sum_for_time <= 0) {
                success = false;
                break;
            }
        }
        if (!success) {
            //no_skip = 3;
            return MethodResult(0ul);
        }
        return MethodResult(1ul);
    }
};

} // namespace pdc

#endif // HPP_3BE77A598E70458ABAF0AA33090ADD60