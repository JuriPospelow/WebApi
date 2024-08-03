#pragma once

#include <string>
#include <map>
#include <boost/asio.hpp>

namespace net = boost::asio;                    // from <boost/asio.hpp>

class WebServer {
    net::ip::address _address{};
    unsigned short _port {};
    std::shared_ptr<std::string> _doc_root{};
    int _threads{};

    typedef std::multimap<std::string, std::string> MultiMap;
    MultiMap DataPrepare;
public:
    explicit WebServer(std::string fileName);

private:
    void readCSV(std::string file_name);
};
