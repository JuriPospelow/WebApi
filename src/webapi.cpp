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

void dataPrepare(std::string key)
{
    std::string data("netlog.csv");

    ifstream in(data.c_str());
    if (!in.is_open()) return ;

    typedef tokenizer< escaped_list_separator<char> > Tokenizer;

    vector< std::string > vec;
    std::string line;

    int cnt_eol = 0;
    MultiMap DataPrepare;

    while (getline(in,line))
    {
        Tokenizer tok(line);
        vec.assign(tok.begin(),tok.end());

        if(line.find("\n")) {
            ++cnt_eol;
            if(cnt_eol == 3){
                // cout << line << " " << endl;
                // cout << cnt_eol << " " << endl;
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
// Make JSON-String
    MultiMap::iterator itr;
    for (itr = DataPrepare.begin(); itr != DataPrepare.end(); ++itr) {
        if(itr->first == key) {
            trim(itr->second);
            std::string out = "[" + itr->second + "]";
            // string out = "[" + trim(itr->second) + "]";
            cout << out << '\n';
        }
    }
}


int main()
{
    dataPrepare("header");
    return 0;
}
