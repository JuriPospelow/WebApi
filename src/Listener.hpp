#pragma once

#include <memory>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "DataWebApi.hpp"

using tcp = boost::asio::ip::tcp;               // from <boost/asio/ip/tcp.hpp>
namespace net = boost::asio;                    // from <boost/asio.hpp>
namespace beast = boost::beast;                 // from <boost/beast.hpp>


// Accepts incoming connections and launches the sessions
class listener : public std::enable_shared_from_this<listener>
{
    net::io_context& ioc_;
    tcp::acceptor acceptor_;
    std::shared_ptr<struct DataWebApi> data_files;

public:
    listener(
        net::io_context& ioc,
        tcp::endpoint endpoint,
        std::shared_ptr<struct DataWebApi> const& data_files
    );

    // Start accepting incoming connections
    void run();

private:
// Report a failure
    void fail(const beast::error_code& ec, char const* what);
    void do_accept();
    void on_accept(const beast::error_code& ec, tcp::socket socket);
};
