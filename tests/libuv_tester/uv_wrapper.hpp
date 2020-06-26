#ifndef HPP_5F6DA8B095D64631B0003809F68A0FFD

#define HPP_5F6DA8B095D64631B0003809F68A0FFD

#include <uv.h>
#include <memory>
#include <functional>

// lib uv c++ wrapper
namespace uvpp {

void free_loop(uv_loop_t* loop);
using unique_loop = std::unique_ptr<uv_loop_t, decltype(free_loop)*>;
unique_loop make_loop();

void free_tcp(uv_tcp_t* socket);
using unique_tcp = std::unique_ptr<uv_tcp_t, decltype(free_tcp)*>;
unique_tcp make_tcp(uv_loop_t* loop);

void free_buf(uv_boof_t* buf);
using unique_buf = std::unique_ptr<uv_boof_t, decltype(free_buf)*>;
unique_buf make_buf(std::size_t size);

sockaddr get_addr(const char* addr, unsigned short port);

template<typename T, typename S>
T*& payload(S* obj) {
    return reinterpret_cast<T*&>(obj->data);
}

struct function_context {
    std::function<void(uv_work_t*)> function;
    void* context;
};

/*
 * Концепция такова, мы получаем указатель на функциональный контекст.
 * Из этого контекста в обработчике достается пользовательский контекст,
 * и функция, которую он хочет запустить. В handle записывается пользовательский
 * контекст и вызывается его функция. В асинхронный обработчик завершения
 * также передается handle с пользовательским контекстом.
 * 
 * Таким образом достигается:
 * 1) Пользователь может запустить функцию и пользоваться в ней своим контекстом, а так же
 *    передать его дальше по цепочке асинхронных вызовов.
 * 2) Обертка не владеет ничем, ей не нужно выделять память ни под что.
 * 3) +\- удобный синтаксис
 */
void async_func(uv_work_t* handle, uv_loop_t* loop, uv_after_work_cb callback);


} // namespace uvpp

#endif // HPP_5F6DA8B095D64631B0003809F68A0FFD
