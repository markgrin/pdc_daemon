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
    auto loop = uvpp::make_loop();

    std::vector<std::unique_ptr<context>> contexts;
    for (std::size_t i = 0; i < threads; i++) {
        contexts.emplace_back(new context(i));
    }
    for (auto& con : contexts) {
        launch(con.get(), loop.get());
    }

    auto timer = std::unique_ptr<uv_timer_t>(new uv_timer_t);
    uv_timer_init(loop.get(), timer.get());
    std::size_t time = 1000 * 2;
    uv_timer_start(timer.get(), on_timer, time, 0);

    uv_run(loop.get(), UV_RUN_DEFAULT);
    for (std::size_t i = 0; i < 100; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (!uv_loop_alive(loop.get())) {
            break;
        }
    }

    std::size_t sum = 0;
    for (auto& con : contexts) {
        sum += con->state;
    }
    std::cout << "MADE:" << sum << " queries\n";

    return 0;
}
