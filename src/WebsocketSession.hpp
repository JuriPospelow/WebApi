#pragma once

#include <memory>
#include <map>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/json/value.hpp>

#include "MultiMap.hpp"

namespace beast = boost::beast;                 // from <boost/beast.hpp>
namespace websocket = beast::websocket;         // from <boost/beast/websocket.hpp>
namespace http = beast::http;                   // from <boost/beast/http.hpp>

using tcp = boost::asio::ip::tcp;               // from <boost/asio/ip/tcp.hpp>
using namespace boost::json;

// Echoes back all received WebSocket messages
class websocket_session : public std::enable_shared_from_this<websocket_session>
{
    websocket::stream<beast::tcp_stream> ws_;
    beast::flat_buffer buffer_;
    MultiMap _log_data;
    MultiMap _ini_data;

public:
    // Take ownership of the socket
    explicit websocket_session(tcp::socket&& socket, MultiMap & log_data, MultiMap & ini_data)
        : ws_(std::move(socket))
        , _log_data(log_data)
        , _ini_data(ini_data)
    {
    }

    // Start the asynchronous accept operation
    template<class Body, class Allocator>
    void do_accept(http::request<Body, http::basic_fields<Allocator>> req);

private:
    void fail(beast::error_code ec, char const* what);
    void on_accept(beast::error_code ec);
    void do_read();
    void on_read(beast::error_code ec, std::size_t bytes_transferred);
    void on_write(beast::error_code ec, std::size_t bytes_transferred);
    value handle_request();
    std::string  read_state();
};

// Start the asynchronous accept operation
template<class Body, class Allocator>
void websocket_session:: do_accept(http::request<Body, http::basic_fields<Allocator>> req)
{
    // Set suggested timeout settings for the websocket
    ws_.set_option(
        websocket::stream_base::timeout::suggested(
            beast::role_type::server));

    // Set a decorator to change the Server of the handshake
    ws_.set_option(websocket::stream_base::decorator(
        [](websocket::response_type& res)
    {
        res.set(http::field::server,
                    std::string(BOOST_BEAST_VERSION_STRING) +
                    " advanced-server");
        }));

    // Accept the websocket handshake
    ws_.async_accept(
        req,
        beast::bind_front_handler(
            &websocket_session::on_accept,
            shared_from_this()));
}
