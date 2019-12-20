#ifndef HPP_43C018E0F6D3463382D960F97F685528

#define HPP_43C018E0F6D3463382D960F97F685528

#include <mutex>
#include <map>
#include <thread>
#include <deque>

namespace pdc {

class session {

    std::deque<std::size_t> setup_;
    std::deque<std::size_t> talk_;
    std::size_t stat_size_;

    public:
    std::mutex lock_; // locking is delegated to caller

    explicit session (std::size_t stat_size)
    :
    stat_size_(stat_size) {
    }

    void add(std::size_t setup, std::size_t talk) {
        setup_.push_back(setup);
        talk_.push_back(talk);
        if (setup_.size() > stat_size_) {
            setup_.pop_front();
            talk_.pop_front();
        }
    }

    std::size_t get(std::size_t free, std::size_t incoming) const {
        return std::max(incoming, free) - incoming; // real predictive dialer would implement some algorithm here
    }

};

class session_manager {
    std::mutex lock_;
    std::map<std::string, session> storage_;

    std::string begin(std::string session, std::size_t stat_size);
    std::string add(std::string session, std::size_t setup, std::size_t talk);
    std::string get(std::string session, std::size_t free, std::size_t incoming);
    std::string end(std::string session);

    public:

    std::string action(std::string string);
};

} // namespace pdc

#endif // HPP_43C018E0F6D3463382D960F97F685528
