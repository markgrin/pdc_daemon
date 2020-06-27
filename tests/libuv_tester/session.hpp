#ifndef HPP_C03FDE1A0AEA4188A722805D2F178977

#define HPP_C03FDE1A0AEA4188A722805D2F178977

#include "uv_wrapper.hpp"

/**
 * Контекст определяющий одну сессию тестирования.
 * Переносится между асинхронными вызовами.
 */
struct context {
    uvpp::unique_tcp socket;
    std::unique_ptr<uv_connect_t> connection;
    std::unique_ptr<uv_shutdown_t> shutdown;
    std::unique_ptr<uv_write_t> write;
    std::unique_ptr<uv_write_t> read;
    std::unique_ptr<uv_work_t> work;
    uvpp::unique_buf read_buf;
    uvpp::unique_buf write_init_buf;
    uvpp::unique_buf write_add_buf;
    uv_loop_t* loop;
    std::size_t index;
    std::size_t state;
    uvpp::function_context func;
    sockaddr endpoint;

    context (std::size_t index_)
    : socket (nullptr, nullptr),
      connection (),
      shutdown (),
      write (),
      read (),
      loop (nullptr),
      index (index_),
      state (0),
      func (),
      read_buf (nullptr, nullptr),
      write_init_buf(nullptr, nullptr),
      write_add_buf(nullptr, nullptr)
      {
          endpoint = uvpp::get_addr("127.0.0.1", 1235);
      }
};

void launch(context* con, uv_loop_t* loop);

#endif // HPP_C03FDE1A0AEA4188A722805D2F178977
