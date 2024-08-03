#include <iostream>
#include <fstream>
#include <vector>

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "Listener.hpp"
#include "WebServer.hpp"

using namespace std;
using namespace boost;

void WebServer::readCSV(std::string file_name){
    std::string data(file_name);

    ifstream in(data.c_str());
    if (!in.is_open()) return ;

    typedef tokenizer< escaped_list_separator<char> > Tokenizer;

    vector< std::string > vec;
    std::string line;

    int cnt_eol = 0;

    while (getline(in,line))
    {
        Tokenizer tok(line);
        vec.assign(tok.begin(),tok.end());

        if(line.find("\n")) {
            ++cnt_eol;
            if(cnt_eol == 3){
                DataPrepare.insert(pair<std::string, std::string>("header", line));
            }
        }

        if (cnt_eol > 3){
            if(line.find(".01.") != std::string::npos) {
                 DataPrepare.insert(pair<std::string, std::string>("January", line));
            }
            else if(line.find(".02.") != std::string::npos){
                 DataPrepare.insert(pair<std::string, std::string>("February", line));
            }
        }

    }
}

WebServer::WebServer(std::string fileName)
{

    boost::property_tree::ptree config;
    std::string file_name{};

    try
    {
        std::cout << "read " << fileName << "\n";

        read_ini(fileName, config);
        file_name = config.get<std::string>("input.log_file");
        _address = net::ip::make_address(config.get<std::string>("net.pc_addr"));
        _port = config.get<unsigned short>("net.api_port");
        _doc_root = std::make_shared<std::string>(config.get<std::string>("net.doc_root"));
        _threads = config.get<int>("opt.threads");

        // std::cout << "opt.threads" << ": " << config.get<std::string>("opt.threads") <<endl;
        // std::cout << "input.log_file" << ": " << config.get<std::string>("input.log_file") <<endl;
        // std::cout << "net.pc_addr" << ": " << config.get<std::string>("net.pc_addr") <<endl;
        // std::cout << "net.api_port" << ": " << config.get<std::string>("net.api_port") <<endl;
        // std::cout << "net.doc_root" << ": " << config.get<std::string>("net.doc_root") <<endl;
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
        _doc_root)->run();

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
