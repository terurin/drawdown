#include "ast.hpp"
#include <sstream>
namespace drawdown {
using namespace std;

ast_ptr ast_builder::get_ast() const {
    if (root == nullptr) {
        root = build();
    }
    return root;
}

ast_ptr ast_builder::build() const {
    auto now = tokens.begin();
    //出落ち
    if ((*now)->type == token_type::end) {
        return nullptr;
    }
    return parse(now);
}

ast_ptr ast_builder::parse(const token_iterator &it)const {
    return parse_add(it);
}

ast_ptr ast_builder::parse_add(const token_iterator &it)const {
    return nullptr;//作ってない
}

ast_ptr ast_builder::parse_tail(const token_iterator &it)const {
    

}

std::wstring to_wstring(ast_type t) {
    switch (t) {
    case ast_type::unknown: return L"unknown";
    case ast_type::add: return L"add";
    case ast_type::sub: return L"sub";
    case ast_type::mul: return L"mul";
    case ast_type::div: return L"div";
    case ast_type::integer: return L"int";
    case ast_type::label: return L"label";
    default: return L"unknown";
    }
}

wstring ast::to_wstring() const {
    wstringstream wss;
    wss << L"(" << drawdown::to_wstring(type);
    wss << L" " << (right?right->to_wstring():L"none");
    wss << L" " << (left?left->to_wstring():L"none") << L")";
    return wss.str();
}

} // namespace drawdown