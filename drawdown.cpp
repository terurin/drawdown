#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "format.hpp"
using namespace std;
using namespace drawdown;

static vector<wstring> load_words();
int main(int argc,char** argv){
    using namespace std;
    wstring line;
    wstringstream ss;
    wcout<<"please input [ctrl + D]"<<endl;
    while( getline(wcin,line)){
        ss<<line<<endl;
    }
    wcout<<"result"<<endl;
    tokener tokenalize(ss.str());
    const auto &list = tokenalize.get_list();
    for (shared_ptr<word>it: list){
        wcout<<it->to_wstring()<<endl;
    }


    return 0;
}


