#pragma once

#include <string>
#include <string_view>
#include <boost/asio.hpp>

#include "DataWebApi.hpp"

namespace net = boost::asio;                    // from <boost/asio.hpp>

class WebServer {
    net::ip::address _address{};
    unsigned short _port {};
    std::shared_ptr<std::string> _doc_root{};
    int _threads{};
    MultiMap _dataPrepare;
    MultiMap _dataIni;
public:
    explicit WebServer(std::string_view fileName);
    void start();

private:
    void readCSV(std::string_view file_name);
};
