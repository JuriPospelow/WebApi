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


void WebServer::readCSV(std::string_view file_name){

    ifstream in(file_name.data());
    if (!in.is_open()) return ;

    vector< std::string > vec;
    std::string line;

    while (getline(in,line))
    {
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end()); // remove ALL spaces ion line
        boost::algorithm::trim_right_if(line, boost::is_any_of(",")); // remove "," on the line end

        std::regex datum_regex("([0-90-9]{2})."
                               "([0-90-9]{2})."
                               "([0-90-9]{2})");
        std::smatch datum_match;

        if(line.find("Datum") != std::string::npos) {
            data_files->dataLog.insert(pair<std::string, std::string>("header", line));
        } else if (std::regex_search(line, datum_match, datum_regex)){
            data_files->dataLog.insert(pair<std::string, std::string>((datum_match[2].str() + "." + datum_match[3].str()), line));
        }
    }
}

WebServer::WebServer(std::string_view fileName)
{

    boost::property_tree::ptree config;
    std::string file_name{};

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
        data_files->dataIni.insert(pair<std::string, std::string>("ini_file", config.get<std::string>("netlog.ini_file")));

    }
    catch (boost::property_tree::ini_parser_error& error)
    {
        std::cout
            << error.message() << ": "
            << error.filename() << ", line "
            << error.line() << std::endl;
    }

    readCSV(file_name);
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
