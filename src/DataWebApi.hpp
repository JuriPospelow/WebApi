#pragma once

#include <map>

typedef std::multimap<std::string, std::string> MultiMap;

struct DataWebApi {
    MultiMap dataLog;
    MultiMap dataIni;
};
