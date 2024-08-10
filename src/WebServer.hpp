#pragma once

#include <string>
#include <map>
#include <boost/asio.hpp>

#include "MultiMap.hpp"

namespace net = boost::asio;                    // from <boost/asio.hpp>

class WebServer {
    net::ip::address _address{};
    unsigned short _port {};
    std::shared_ptr<std::string> _doc_root{};
    int _threads{};
    MultiMap _dataPrepare;
    MultiMap _dataIni;
public:
    explicit WebServer(std::string fileName);
    void start();

private:
    void readCSV(std::string file_name);
};
