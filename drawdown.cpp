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
    auto list = load_words(ss.str());
    for (auto &it: list){
        wcout<<it<<L',';
    }


    return 0;
}


