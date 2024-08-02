#include <iostream>     // cout, endl
#include <fstream>      // fstream
#include <vector>
#include <map>
#include <string>

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

typedef multimap<std::string, std::string> MultiMap;
MultiMap DataPrepare;

void dataPrepare(std::string file_name)
{
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


int main()
{
    dataPrepare("netlog.csv");

    // MultiMap::iterator itr;
    // for (itr = DataPrepare.begin(); itr != DataPrepare.end(); ++itr) {
    //     trim(itr->second);
    //     std::string out = "[" + itr->second + "]";
    //     cout << out << '\n';
    // }


    // readData4Log(file);
    // startWebServer();


    return 0;
}
