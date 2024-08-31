#include <iostream>
#include <fstream>
#include <vector>
#include <regex>

#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "Listener.hpp"
#include "WebServer.hpp"

using namespace std;
using namespace boost;


WebServer::WebServer(std::string_view fileName)
{

    boost::property_tree::ptree config;
    std::string file_name;

    data_files =std::make_shared<struct DataWebApi>(_data);

    try
    {
        std::cout << "read " << fileName << "\n";

        read_ini(fileName.data(), config);
        file_name = config.get<std::string>("netlog.log_file");
        data_files->dataIni.insert(pair<std::string, std::string>("log_file", config.get<std::string>("netlog.log_file")));

        _address = net::ip::make_address(config.get<std::string>("net.pc_addr"));
        data_files->dataIni.insert(pair<std::string, std::string>("pc_addr", config.get<std::string>("net.pc_addr")));

        _port = config.get<unsigned short>("net.api_port");
        data_files->dataIni.insert(pair<std::string, std::string>("api_port", config.get<std::string>("net.api_port")));

        data_files->dataIni.insert(pair<std::string, std::string>("doc_root", config.get<std::string>("net.doc_root")));

        _threads = config.get<int>("opt.threads");
        data_files->dataIni.insert(pair<std::string, std::string>("threads", config.get<std::string>("opt.threads")));

        data_files->dataIni.insert(pair<std::string, std::string>("programm", config.get<std::string>("netlog.programm")));
        data_files->dataIni.insert(pair<std::string, std::string>("appiIni", config.get<std::string>("netlog.appiIni")));
        data_files->dataIni.insert(pair<std::string, std::string>("networkIni", config.get<std::string>("netlog.networkIni")));

    }
    catch (boost::property_tree::ini_parser_error& error)
    {
        std::cout
            << error.message() << ": "
            << error.filename() << ", line "
            << error.line() << std::endl;
    }

}

void WebServer::start(){

    // The io_context is required for all I/O
    net::io_context ioc{_threads};

    // Create and launch a listening port
    std::make_shared<listener>(
        ioc,
        tcp::endpoint{_address, _port},
        data_files)->run();

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
    v.reserve(_threads - 1);
    for(auto i = _threads - 1; i > 0; --i)
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
}
