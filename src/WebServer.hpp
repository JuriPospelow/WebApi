#pragma once

#include <string>
#include <map>

class WebServer {
public:
    explicit WebServer(std::string fileName);

private:
    typedef std::multimap<std::string, std::string> MultiMap;
    MultiMap DataPrepare;
};
