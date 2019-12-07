#ifndef HPP_43C018E0F6D3463382D960F97F685528

#define HPP_43C018E0F6D3463382D960F97F685528

#include <mutex>
#include <map>
#include <thread>

namespace pdc {

class session {

    std::size_t statistics = 0u;
    std::mutex lock;

    public:

    void update_statistics () {
        std::lock_guard guard(lock);
        statistics++;
    }

};

class session_manager {
    std::mutex lock;
    std::map<std::string, session> storage;

    public:

    std::string action(const std::string& string) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return "{ \"result\" : true }";
    }
};

} // namespace pdc

#endif // HPP_43C018E0F6D3463382D960F97F685528
