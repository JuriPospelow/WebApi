#include <iostream>

#include "WebServer.hpp"


int main(int argc, char* argv[])
{
    std::string fileName {"dummy.ini"};

    if (argc >= 2 && argc < 3) {
        fileName = argv[1];
    } else {
        std::cerr <<
            "Usage: webapi <ini file> \n" <<
            "Example:\n" <<
            "    webapi dummy.ini\n";
        return EXIT_FAILURE;
    }

    WebServer web(fileName);
    web.start();

    return EXIT_SUCCESS;

}
