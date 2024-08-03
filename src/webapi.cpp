#include <iostream>
#include <thread>
#include <vector>

#include "WebServer.hpp"
#include "HTTPSession.hpp"
#include "Listener.hpp"


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
#if 0




    // MultiMap::iterator itr;
    // for (itr = DataPrepare.begin(); itr != DataPrepare.end(); ++itr) {
    //     trim(itr->second);
    //     std::string out = "[" + itr->second + "]";
    //     cout << out << '\n';
    // }

   // Check command line arguments.
    if (argc != 5)
    {
        std::cerr <<
            "Usage: webapi <address> <port> <doc_root> <threads>\n" <<
            "Example:\n" <<
            "    webapi 0.0.0.0 8080 . 1\n";
        return EXIT_FAILURE;
    }
    auto const address = net::ip::make_address(argv[1]);
    auto const port = static_cast<unsigned short>(std::atoi(argv[2]));
    auto const doc_root = std::make_shared<std::string>(argv[3]);
    auto const threads = std::max<int>(1, std::atoi(argv[4]));

    // The io_context is required for all I/O
    net::io_context ioc{threads};

    // Create and launch a listening port
    std::make_shared<listener>(
        ioc,
        tcp::endpoint{address, port},
        doc_root)->run();

    // Capture SIGINT and SIGTERM to perform a clean shutdown
    net::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait(
        [&](beast::error_code const&, int)
        {
            // Stop the `io_context`. This will cause `run()`
            // to return immediately, eventually destroying the
            // `io_context` and all of the sockets in it.
            ioc.stop();
        });

    // Run the I/O service on the requested number of threads
    std::vector<std::thread> v;
    v.reserve(threads - 1);
    for(auto i = threads - 1; i > 0; --i)
        v.emplace_back(
        [&ioc]
        {
            ioc.run();
        });
    ioc.run();

    // (If we get here, it means we got a SIGINT or SIGTERM)

    // Block until all the threads exit
    for(auto& t : v)
        t.join();
#endif
    return EXIT_SUCCESS;
    // readData4Log(file);
    // startWebServer();


    // return 0;
}