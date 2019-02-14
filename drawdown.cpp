#include "ast.hpp"
#include "token.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
using namespace drawdown;

static vector<wstring> load_words();
int main(int argc, char **argv) {
    using namespace std;
    wstring line;
    wstringstream ss;
    wcout << "please input [ctrl + D]" << endl;
    while (getline(wcin, line)) { ss << line << L'\n'; }
    wcout << "result" << endl;
    token_builder tokenalize(ss.str());
    const auto &list = tokenalize.get_list();
    for (shared_ptr<token> it : list) { wcout << it->to_wstring() << endl; }

    ast_builder builder(tokenalize.get_list());
    if (builder.get_ast() != nullptr) {
        wcout << builder.get_ast()->to_wstring() << endl;
    }
    return 0;
}
