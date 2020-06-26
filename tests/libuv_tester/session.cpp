#include "session.hpp"

#include <iostream>
#include "message.hpp"

namespace {

void restart (context*);

void on_close (uv_handle_t* shutdown) {
    restart(uvpp::payload<context>(shutdown));
}

template<typename T>
void do_shutdown (context* con, T* any_handle) {
    uv_handle_t* handle = reinterpret_cast<uv_handle_t*>(any_handle);

    uvpp::payload<context>(handle) = con;
    uv_close(reinterpret_cast<uv_handle_t*>(handle), on_close);
}

void alloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = new char[suggested_size];
    buf->len = suggested_size;
}

void on_read (uv_stream_t* read, ssize_t nread, const uv_buf_t* buf) {
    if (nread < 0 || nread == UV_EOF) {
        uvpp::payload<context>(read)->state += 1;
        uv_read_stop(read);
        do_shutdown(uvpp::payload<context>(read), uvpp::payload<context>(read)->socket.get());
    }
}

void on_write(uv_write_t* req_con, int status) {
    auto cont = uvpp::payload<context>(req_con);

    uv_read_start(req_con->handle, alloc, on_read);
}

void write_callback(uv_work_t* handle, int status) {
    auto con = uvpp::payload<context>(handle);
    auto req_con = con->connection.get();
    uv_write(con->write.get(), (uv_stream_t*)req_con->handle, &buf, 1, on_write);
    uvpp::payload<context>(req_con->handle) = con;
    uvpp::payload<context>(con->write.get()) = con;
}

void prepare_write(uv_work_t* work) {
    auto con = uvpp::payload<context>(work);
    auto req_con = con->connection.get();

    auto buf = get_message(con->index, con->state);

}

void on_connect(uv_connect_t* req_con, int status) {
    auto con = uvpp::payload<context>(req_con);
    if (status < 0) {
        do_shutdown(con, con->socket.get());
    }
    con->func.function = do_write;
    uvpp::payload<uvpp::function_context>(con->work.get()) = &(con->func);
    uvpp::async_func(con->work.get(), con->loop, empty_callback);
}

void restart (context* con) {
    uv_tcp_init(con->loop, con->socket.get());
    uv_tcp_connect(con->connection.get(), con->socket.get(), &con->endpoint, on_connect);
}

} // namespace



void launch(context* con, uv_loop_t* loop) {
    con->loop = loop;
    con->connection = std::unique_ptr<uv_connect_t>(new uv_connect_t);
    con->shutdown = std::unique_ptr<uv_shutdown_t>(new uv_shutdown_t);
    con->write = std::unique_ptr<uv_write_t>(new uv_write_t);
    con->shutdown = std::unique_ptr<uv_shutdown_t>(new uv_shutdown_t);
    con->work = std::unique_ptr<uv_work_t>(new uv_work_t);
    con->socket = uvpp::make_tcp(loop);
    con->func.context = reinterpret_cast<void*>(con);

    uvpp::payload<context>(con->connection.get()) = con;
    uvpp::payload<context>(con->shutdown.get()) = con;

    restart(con);
}