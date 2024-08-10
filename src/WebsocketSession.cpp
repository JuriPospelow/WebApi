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

#include <boost/process/system.hpp>
#include <boost/process/io.hpp>
#include <boost/asio/io_service.hpp>
#include <fstream>

#include <iomanip>
#include <chrono>
#include <ctime>

std::string  websocket_session::read_state()
{
    namespace bp = boost::process;

    std::string prog = _ini_data.find("programm")->second;
    std::string arg = _ini_data.find("ini_file")->second;
    bp::system(prog, arg);

    std::ifstream ini_file (_ini_data.find("log_file")->second);
    std::string ini_line;

    if (ini_file.is_open() ) {
        while (ini_file.good()) {
            ini_file >> ini_line;
            std::cout  <<"read_state: " << ini_line << std::endl;
        }
    }

    std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm ltime;
    localtime_r(&t, &ltime);
    std::stringstream transTime;
    transTime << std::put_time(&ltime, "%H:%M:%S");
    // std::cout << std::put_time(&ltime, "%H:%M:%S") << '\n';

    boost::algorithm::trim_right_if(ini_line, boost::is_any_of(","));
    return (transTime.str() + "," + ini_line);
}

//ToDo: improve algorithm
value websocket_session::handle_request(){
    std::string out{};
    std::vector<std::string> words;
    std::vector<std::vector<std::string>> vector_words;
    value jv{};
    MultiMap::iterator itr;

    itr = _log_data.find("header");
    out = itr->first + "," + itr->second;
    boost::split(words, out, boost::is_any_of(","), boost::token_compress_on);

    if(boost::beast::buffers_to_string(buffer_.data()) == "actual") vector_words.push_back(std::vector<std::string> {"actual"});
    vector_words.push_back(words);
    if(boost::beast::buffers_to_string(buffer_.data()) == "actual") {
        boost::split(words, read_state(), boost::is_any_of(","), boost::token_compress_on);
        vector_words.push_back(words);
        jv = value_from(vector_words);
        return jv;
    }


    for (itr = _log_data.begin(); itr != _log_data.end(); ++itr) {
        if(itr->first == boost::beast::buffers_to_string(buffer_.data())){
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
    if(ec == websocket::error::closed) return;

    if(ec) fail(ec, "read");

    auto msg  = serialize(handle_request());

    // This sets all outgoing messages to be sent as text.
    ws_.text(ws_.got_text());

    ws_.async_write(
        std::move(net::buffer(msg)),
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
