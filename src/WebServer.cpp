#include <iostream>
#include <fstream>
#include <vector>
#include <regex>

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

    // typedef tokenizer< escaped_list_separator<char> > Tokenizer;

    vector< std::string > vec;
    std::string line;

    while (getline(in,line))
    {
        // Tokenizer tok(line);
        // vec.assign(tok.begin(),tok.end());

        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end()); // remove ALL spaces ion line
        boost::algorithm::trim_right_if(line, boost::is_any_of(",")); // remove "," on the line end
        // std::cout<<line<<std::endl;

        std::regex datum_regex("([0-90-9]{2})."
                               "([0-90-9]{2})."
                               "([0-90-9]{2})");
        std::smatch datum_match;

        if(line.find("Datum") != std::string::npos) {
             _dataPrepare.insert(pair<std::string, std::string>("header", line));
        } else if (std::regex_search(line, datum_match, datum_regex)){
            _dataPrepare.insert(pair<std::string, std::string>((datum_match[2].str() + "." + datum_match[3].str()), line));
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
        file_name = config.get<std::string>("netlog.log_file");
        _dataIni.insert(pair<std::string, std::string>("log_file", config.get<std::string>("netlog.log_file")));

        _address = net::ip::make_address(config.get<std::string>("net.pc_addr"));
        _dataIni.insert(pair<std::string, std::string>("pc_addr", config.get<std::string>("net.pc_addr")));

        _port = config.get<unsigned short>("net.api_port");
        _dataIni.insert(pair<std::string, std::string>("api_port", config.get<std::string>("net.api_port")));

        _doc_root = std::make_shared<std::string>(config.get<std::string>("net.doc_root"));
        _dataIni.insert(pair<std::string, std::string>("doc_root", config.get<std::string>("net.doc_root")));

        _threads = config.get<int>("opt.threads");
        _dataIni.insert(pair<std::string, std::string>("threads", config.get<std::string>("opt.threads")));

        _dataIni.insert(pair<std::string, std::string>("programm", config.get<std::string>("netlog.programm")));
        _dataIni.insert(pair<std::string, std::string>("ini_file", config.get<std::string>("netlog.ini_file")));

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
        _doc_root, _dataPrepare, _dataIni)->run();

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
