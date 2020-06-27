#include <stdio.h>
#include <stdlib.h>
#include <uv.h>
#include <string.h>
#include <chrono>
#include <thread>
#include <vector>

#include "uv_wrapper.hpp"
#include "session.hpp"
#include <iostream>


void on_timer(uv_timer_t* handle) {
    uv_stop(handle->loop);
}


int main (int argc, char** argv) {
    std::size_t threads = 1;
    if (argc > 1) {
        threads = std::atoi(argv[1]);
    }

    std::vector<std::unique_ptr<context>> contexts;
    std::vector<uvpp::unique_loop> loops;
    std::vector<std::unique_ptr<uv_timer_t>> timers;
    for (std::size_t i = 0; i < threads; i++) {
        contexts.emplace_back(new context(i));
        loops.push_back(uvpp::make_loop());
        timers.emplace_back(new uv_timer_t);
    }

    std::size_t time = 1000 * 10;
    for (std::size_t i = 0; i < loops.size(); i++) {
        launch(contexts[i].get(), loops[i].get());
        uv_timer_init(loops[i].get(), timers[i].get());
        uv_timer_start(timers[i].get(), on_timer, time, 0);
    }


    for (auto& loop : loops) {
        uv_run(loop.get(), UV_RUN_DEFAULT);
    }
    while (!loops.empty()) {
        auto& loop = loops.back();
        for (std::size_t i = 0; i < 100; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            if (!uv_loop_alive(loop.get())) {
                break ;
            }
        }
        loops.pop_back();
    }

    std::size_t sum = 0;
    for (auto& con : contexts) {
        sum += con->state;
    }
    std::cout << "MADE:" << static_cast<std::size_t>(static_cast<double>(sum) / (time / 1000)) << " queries per second\n";

    return 0;
}
