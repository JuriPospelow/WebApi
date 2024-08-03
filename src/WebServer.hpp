#pragma once

#include <string>
#include <map>
#include <boost/asio.hpp>

namespace net = boost::asio;                    // from <boost/asio.hpp>

typedef std::multimap<std::string, std::string> MultiMap;

class WebServer {
    net::ip::address _address{};
    unsigned short _port {};
    std::shared_ptr<std::string> _doc_root{};
    int _threads{};
    MultiMap _dataPrepare;
public:
    explicit WebServer(std::string fileName);
    void start();

private:
    void readCSV(std::string file_name);
};
