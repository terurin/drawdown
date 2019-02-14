#include "ast.hpp"
#include <iostream>
#include <sstream>
namespace drawdown ::ast {

using namespace std;
using namespace token;
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

ast_ptr ast_builder::parse(token_iterator &it) const { return parse_set(it); }

ast_ptr ast_builder::parse_set(token_iterator &it) const {
    if ((*it)->type == token_type::end) {
        return nullptr;
    }
    auto node = parse_add_sub(it);
    for (;;) {
        switch ((*it)->type) {
        case token_type::set:
            node = make_shared<ast>(ast_type::set, node, parse_mul_div(++it));
            break;
        default: return node;
        }
    }
}

ast_ptr ast_builder::parse_add_sub(token_iterator &it) const {
    if ((*it)->type == token_type::end) {
        return nullptr;
    }
    auto node = parse_mul_div(it);
    for (;;) {
        switch ((*it)->type) {
        case token_type::add:
            node = make_shared<ast>(ast_type::add, node, parse_mul_div(++it));
            break;
        case token_type::sub:
            node = make_shared<ast>(ast_type::sub, node, parse_mul_div(++it));
            break;
        default: return node;
        }
    }
}

ast_ptr ast_builder::parse_mul_div(token_iterator &it) const {
    if ((*it)->type == token_type::end) {
        return nullptr;
    }
    auto node = parse_tail(it);
    for (;;) {
        switch ((*it)->type) {
        case token_type::mul:
            node = make_shared<ast>(ast_type::mul, node, parse_tail(++it));
            break;
        case token_type::div:
            node = make_shared<ast>(ast_type::div, node, parse_tail(++it));
            break;
        default: return node;
        }
    }
}

ast_ptr ast_builder::parse_tail(token_iterator &it) const {
    if ((*it)->type == token_type::end) {
        return nullptr;
    }
    auto type = (*it)->type;
    if (type == token_type::bracket_begin) {
        auto internal = parse(++it);
        auto is_exit = (*it)->type == token_type::bracket_end;
        if (is_exit) {
            return make_shared<ast>(ast_type::bracket, internal);
        } else {
            wcout << L"Error:miss )" << endl;
            return nullptr;
        }
    }
    switch (type) {
    case token_type::label:
        if (auto token = dynamic_pointer_cast<label>(*it++)) {
            return make_shared<ast_label>(token->value);
        } else {
            return nullptr;
        }
    case token_type::integer:
        if (auto token = dynamic_pointer_cast<integer>(*it++)) {
            return make_shared<ast_int>(token->value);
        } else {
            return nullptr;
        }
    default: wcout << L"Error: miss value " << endl; return nullptr;
    }
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
    case ast_type::bracket: return L"bracket";
    case ast_type::exp: return L"exp";
    case ast_type::para: return L"para";
    case ast_type::set: return L"set";
    default: return L"unknown";
    }
}

wstring ast::to_wstring() const {
    wstringstream wss;
    wss << L"(" << drawdown::ast::to_wstring(type);
    if (right != nullptr) {
        wss << L" " << right->to_wstring();
        if (left != nullptr) {
            wss << L" " << left->to_wstring();
        }
    }
    wss << L")";
    return wss.str();
}

wstring ast_label::to_wstring() const {
    wstringstream wss;
    wss << L"(" << drawdown::ast::to_wstring(type);
    wss << L" " << label << L")";
    return wss.str();
}

wstring ast_int::to_wstring() const {
    wstringstream wss;
    wss << L"(" << drawdown::ast::to_wstring(type);
    wss << L" " << value << L")";
    return wss.str();
}

} // namespace drawdown::ast