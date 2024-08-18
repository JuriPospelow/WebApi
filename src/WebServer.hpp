#pragma once

#include <string_view>
#include <boost/asio.hpp>

#include "DataWebApi.hpp"

namespace net = boost::asio;                    // from <boost/asio.hpp>

class WebServer {
    struct DataWebApi _data{};
    net::ip::address _address{};
    unsigned short _port {};
    std::shared_ptr<struct DataWebApi> data_files{};
    int _threads{};

public:
    explicit WebServer(std::string_view fileName);
    void start();

private:
    void readCSV(std::string_view file_name);
};
