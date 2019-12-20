#include "server.hpp"

#include "session.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <list>
#include <memory>
#include <string>


namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

namespace pdc {

struct worker {

    worker(worker const&) = delete;
    worker& operator=(worker const&) = delete;

    explicit worker(net::io_context& ioc, tcp::acceptor& acceptor, session_manager& manager)
        :
    session_manager_ (manager),
    socket_ (ioc),
    acceptor_ (acceptor),
    ioc_ (ioc)
    {}

    session_manager& session_manager_;
    tcp::socket socket_;
    tcp::acceptor& acceptor_;
    net::io_context& ioc_;

    void start() {
        accept();
    }

    private:

    void accept () {
         beast::error_code ec;
         socket_.close(ec);
         buffer_.consume(buffer_.size());

         acceptor_.async_accept(
             socket_,
             [this](beast::error_code ec)
             {
                 if (ec)
                 {
                     accept();
                 }
                 else
                 {
                     read_request();
                 }
             });
    }

    boost::optional<http::parser<true, http::buffer_body>> parser_;

    beast::flat_static_buffer<8192> buffer_;

    void read_request() {
        parser_.emplace();
        buffer_.reset();

        http::async_read(
            socket_,
            buffer_,
            *parser_,
            [this](beast::error_code ec, std::size_t)
            {
                if (ec) {
                    accept();
                }
                else {
                    process_request(parser_->get());
                }
            });
    }

    void process_request(http::request<http::buffer_body> const& req) {
        req.body();
        send_response(session_manager_.action(std::string(req.target())));
    }


    std::optional<http::response<http::string_body>> string_response_;
    std::optional<http::response_serializer<http::string_body>> string_serializer_;
    void send_response(
        std::string const& payload)
    {
        string_response_.emplace();

        string_response_->result(http::status::ok);
        string_response_->keep_alive(false);
        string_response_->set(http::field::server, "Beast");
        string_response_->set(http::field::content_type, "text/plain");
        string_response_->body() = payload;
        string_response_->prepare_payload();

        string_serializer_.emplace(*string_response_);

        http::async_write(
            socket_,
            *string_serializer_,
            [this](beast::error_code ec, std::size_t)
            {
                socket_.shutdown(tcp::socket::shutdown_send, ec);
                accept();
            });
    }

};

void start(config conf)
{
    std::cout << "Starting server\n";
    auto const address = net::ip::make_address("127.0.0.1");
    unsigned short port = static_cast<unsigned short>(conf.port);

    session_manager manager;

    net::io_context ioc(10);
    boost::asio::ip::tcp::acceptor acceptor(ioc);
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
    acceptor.open(endpoint.protocol());
    acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor.bind(endpoint);
    acceptor.listen();

    std::list<worker> list;
    std::cout << "Creating workers\n";
    for (std::size_t i = 0; i < 10; i++) {
        list.emplace_back(ioc, acceptor, manager);
        list.back().start();
    }

    ioc.run();
}

} // namespace pdc
