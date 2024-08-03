#include <iostream>
#include "WebsocketSession.hpp"
#include <boost/json/src.hpp> // for header-only
#include <boost/json/value.hpp>

namespace net = boost::asio;                    // from <boost/asio.hpp>
// namespace json = boost::json;

using namespace boost::json;


// Report a failure
void websocket_session::fail(beast::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}

void websocket_session::on_accept(beast::error_code ec)
{
    if(ec)
        return fail(ec, "accept");

    // Read a message
    do_read();
}

void websocket_session::do_read()
{
    // Read a message into our buffer
    ws_.async_read(
        buffer_,
        beast::bind_front_handler(
            &websocket_session::on_read,
            shared_from_this()));
}

void websocket_session::on_read(beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    // This indicates that the websocket_session was closed
    if(ec == websocket::error::closed)
        return;

    if(ec)
        fail(ec, "read");

    // Echo the message
    ws_.text(ws_.got_text());
    // if (boost::beast::buffers_to_string(buffer_.data()) == "Hello") {
    //     std::cout <<  boost::beast::buffers_to_string(buffer_.data()) << std::endl;
    // } else if (boost::beast::buffers_to_string(buffer_.data()) == "header"){
    //      buffer_.data() = "[servix, xeonix, gigantix, duomensix3, servix2, commandix, asterix, tc2, aoi2]"
    // }
    std::vector< std::string > v1{ "servix2", "commandix" };
    value jv = value_from( v1 );
    ws_.async_write(
        // buffer_.data(),
        net::buffer(serialize(jv)),
        // net::buffer("[\"servix\", \"xeonix\", \"gigantix\", \"duomensix3\", "servix2", "commandix", "asterix", "tc2", "aoi2"]"),
        // net::buffer(json::value_from("[1,2,3]")),
        // net::buffer(json::value_from("[\"servix\", \"xeonix\", \"gigantix\", \"duomensix3\"]")),
        beast::bind_front_handler(
            &websocket_session::on_write,
            shared_from_this()));
}

void websocket_session::on_write(beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if(ec)
        return fail(ec, "write");

    // Clear the buffer
    buffer_.consume(buffer_.size());

    // Do another read
    do_read();
}
