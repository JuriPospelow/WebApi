#include <iostream>
#include "WebsocketSession.hpp"
#include <boost/json/src.hpp>
#include <boost/algorithm/string.hpp>

namespace net = boost::asio;                    // from <boost/asio.hpp>

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


value websocket_session::make_json(){
    std::string out{};
    std::vector<std::string> words;
    std::vector<std::vector<std::string>> vector_words;
    value jv{};
    MultiMap::iterator itr;

    if(boost::beast::buffers_to_string(buffer_.data()) == "actual"){
        std::cout << "ACTUAL"<< std::endl;
        return jv;
    }

    // if(boost::beast::buffers_to_string(buffer_.data()) == "header"){
        itr = _log_data.find("header");
        boost::algorithm::trim(itr->second); //ToDo: remove all Spaces
        boost::algorithm::trim_right_if(itr->second, boost::is_any_of(","));
        out = itr->first + "," + itr->second;
        boost::split(words, out, boost::is_any_of(","), boost::token_compress_on);
        vector_words.push_back(words);
    //     jv = value_from(words);
    //     return jv;
    // }

    for (itr = _log_data.begin(); itr != _log_data.end(); ++itr) {
        if(itr->first == boost::beast::buffers_to_string(buffer_.data())){
                boost::algorithm::trim(itr->second); //ToDo: remove all Spaces
                boost::algorithm::trim_right_if(itr->second, boost::is_any_of(","));
                boost::split(words, itr->second, boost::is_any_of(","), boost::token_compress_on);
                vector_words.push_back(words);
                jv = value_from(vector_words);
        }
    }
    return jv;
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

    ws_.async_write(
        net::buffer(serialize(make_json())),
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
