#pragma once

#include <memory>
#include <queue>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include "DataWebApi.hpp"

namespace beast = boost::beast;                 // from <boost/beast.hpp>
namespace http = beast::http;                   // from <boost/beast/http.hpp>
using tcp = boost::asio::ip::tcp;               // from <boost/asio/ip/tcp.hpp>

// Handles an HTTP server connection
class http_session : public std::enable_shared_from_this<http_session>
{
    beast::tcp_stream stream_;
    beast::flat_buffer buffer_;
    std::shared_ptr<struct DataWebApi> & data_files;

    static constexpr std::size_t queue_limit = 8; // max responses
    std::queue<http::message_generator> response_queue_;


    // The parser is stored in an optional container so we can
    // construct it from scratch it at the beginning of each new message.
    boost::optional<http::request_parser<http::string_body>> parser_;

public:
    // Take ownership of the socket
    http_session(
        tcp::socket&& socket,
        std::shared_ptr<struct DataWebApi> & data_files
        )
        : stream_(std::move(socket))
        , data_files(data_files)
    {
        static_assert(queue_limit > 0,
                      "queue limit must be positive");
    }

    // Start the session
    void run();

private:
    void fail(beast::error_code ec, char const* what);
    void do_read();
    void on_read(beast::error_code ec, std::size_t bytes_transferred);
    void queue_write(http::message_generator response);

    // Called to start/continue the write-loop. Should not be called when
    // write_loop is already active.
    void do_write();
    void on_write(
        bool keep_alive,
        beast::error_code ec,
        std::size_t bytes_transferred);
    void do_close();
};
