#include "uv_wrapper.hpp"
#include <iostream>

#include <stdexcept>

namespace uvpp {

void free_loop(uv_loop_t* loop) {
    if(loop) {
        uv_loop_close(loop);
    }
    delete loop;
}

unique_loop make_loop() {
    uv_loop_t *loop = new uv_loop_t;
    if (uv_loop_init(loop) < 0) {
        delete loop;
        throw std::runtime_error("loop error");
    }
    return unique_loop(loop, free_loop);
}

void free_tcp(uv_tcp_t* socket) {
    delete socket;
}

unique_tcp make_tcp(uv_loop_t* loop) {
    if (!loop) {
        return unique_tcp(nullptr, free_tcp);
    }

    uv_tcp_t* socket = new uv_tcp_t;
    if (uv_tcp_init(loop, socket) < 0) {
        delete loop;
        throw std::runtime_error("tcp error");
    }
    return unique_tcp(socket, free_tcp);
}

void free_buf(uv_buf_t* buf) {
    if (buf) {
        delete[] buf->base;
    }
    delete buf;
}

unique_buf make_buf(std::size_t size) {
    uv_buf_t* res = new uv_buf_t;
    *res = uv_buf_init(new char[size], size);
    return unique_buf(res, free_buf);
}

sockaddr get_addr(const char* addr, unsigned short port) {
    sockaddr_in tmp;
    uv_ip4_addr(addr, port, &tmp);
    return *reinterpret_cast<sockaddr*>(&tmp);
}

void work_cb (uv_work_t* req) {
    auto con = uvpp::payload<function_context>(req);
    uvpp::payload<void>(req) = con->context;

    con->function(req);
}

void async_func(uv_work_t* handle, uv_loop_t* loop, uv_after_work_cb callback) {
    uv_queue_work(loop, handle, work_cb, callback);
}

} // namespace uvpp
