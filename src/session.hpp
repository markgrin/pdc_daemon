#ifndef HPP_43C018E0F6D3463382D960F97F685528

#define HPP_43C018E0F6D3463382D960F97F685528

#include <mutex>
#include <map>
#include <thread>
#include <deque>
#include "grin.hpp"
#include "berlang.hpp"

namespace pdc {

#define PROGRESSIVE_SESSION

#ifdef PROGRESSIVE_SESSION

/**
 * Session - stores, updates statistics. Calculates number of calls to make based on statistics.
 */
class session {

    public:
    std::mutex lock_; /// locking is delegated to caller

    explicit session (double critical) {
    }

    void add(double setup, double talk) {
        return ;
    }

    void addBusy(double setup) {
        return ;
    }

    double get(std::size_t agents, std::vector<double> setup, std::vector<double> service) {
        return std::max(agents, setup.size()) - setup.size();
    }

};
#endif // PROGRESSIVE_SESSION



#ifdef BERLANG_SESSION

/**
 * Session - stores, updates statistics. Calculates number of calls to make based on statistics.
 */
class session {

    Berlang berlang;

    public:
    std::mutex lock_; /// locking is delegated to caller

    void add(double setup, double talk) {
        Call call{true, setup, talk};
        berlang.addCall(call);
    }

    session(double critical) {
        berlang.set_critical(critical);
    }

    void addBusy(double setup) {
        berlang.addBusy(setup);
    }

    double get(std::size_t agents, std::vector<double> setup, std::vector<double> service) {
        return berlang.calculate(agents, setup, service).rate;
    }

};

#endif // BERLANG_SESSION


#ifdef GRIN_SESSION

/**
 * Session - stores, updates statistics. Calculates number of calls to make based on statistics.
 */
class session {

    Grin grin;

    public:
    std::mutex lock_; /// locking is delegated to caller

    session(double critical) {
        grin.set_critical(critical);
    }

    void add(double setup, double talk) {
        Call call{true, setup, talk};
        grin.addCall(call);
    }

    void addBusy(double setup) {
        grin.addBusy(setup);
    }

    double get(std::size_t agents, std::vector<double> setup, std::vector<double> service) {
        return grin.calculate(agents, setup, service).calls;
    }

};



#endif // GRIN_SESSION

/**
 * Session manager. It stores, manages sessions. Directs reqeusts to them.
 */
class session_manager {
    std::mutex lock_;
    std::map<std::string, session> storage_;

    std::string begin(std::string session, std::size_t stat_size);
    std::string add(std::string session, std::size_t setup, std::size_t talk);
    std::string get(std::string session, std::size_t free, std::size_t incoming);
    std::string end(std::string session);

    public:

    /**
     * Handles request
     * @param string uri of the request
     * @return result of handling
     */
    std::string action(std::string string);
};

} // namespace pdc

#endif // HPP_43C018E0F6D3463382D960F97F685528
