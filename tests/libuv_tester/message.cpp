#include "message.hpp"
#include <iostream>
#include <string.h>


namespace {

const char* CREATE_MESSAGE = "GET /begin?session=0000&stat_size=200 HTTP/1.1\r\n\
Host: 127.0.0.1:1235\r\n\
User-Agent: python-requests/2.23.0\r\n\
Accept-Encoding: gzip, deflate\r\n\
Accept: */*\r\n\
Connection: keep-alive\r\n\
\r\n";

const char* STAT_MESSAGE = "GET /add?session=0000&agents=20&setup=10&service=8 HTTP/1.1\r\n\
Host: 127.0.0.1:1235\r\n\
User-Agent: python-requests/2.23.0\r\n\
Accept-Encoding: gzip, deflate\r\n\
Accept: */*\r\n\
Connection: keep-alive\r\n\
\r\n";

} // namespace

uvpp::unique_buf get_message(std::size_t index, std::size_t state) {
    std::size_t length = (state > 0 ? 190 : 178);
    std::size_t dif = 2;
    auto result = uvpp::make_buf(length);
    const char* msg = CREATE_MESSAGE;
    if (state > 0) {
        dif = 0;
        msg = STAT_MESSAGE;
    }
    std::copy(msg, msg + length, result->base);
    result->base[18 + dif] = '0' - static_cast<char>(index % 10);
    result->base[19 + dif] = '0' - static_cast<char>(index / 10);

    return result;
}
