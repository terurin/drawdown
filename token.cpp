#include "token.hpp"
#include <cwctype>
#include <math.h>
#include <sstream>
#include <stdlib.h>
#include <iostream>
namespace drawdown {
using namespace std;
std::wstring to_wstring(token_type token) {
    switch (token) {
    case token_type::label: return L"label";
    case token_type::integer: return L"integer";
    case token_type::real: return L"real";
    case token_type::add: return L"add";
    case token_type::sub: return L"sub";
    case token_type::mul: return L"mul";
    case token_type::exp: return L"exp";
    case token_type::div: return L"div";
    case token_type::para: return L"para";
    case token_type::dot: return L"dot";
    case token_type::bracket_begin: return L"(";
    case token_type::bracket_end: return L")";
    case token_type::block_begin: return L"{";
    case token_type::block_end: return L"}";
    case token_type::newline: return L"newline";
    case token_type::end: return L"end";
    case token_type::keyword_frame: return L"frame";
    default: return L"unknown";
    }
}

std::wstring token::to_wstring() const {
    wstringstream ss;
    ss << L"type:" << drawdown::to_wstring(type);
    return ss.str();
}

bool token::operator==(const token &cmp) const { return type == cmp.type; }

std::wstring label::to_wstring() const {
    wstringstream ss;
    ss << L"type:" << drawdown::to_wstring(type) << ",value:" << value;
    return ss.str();
}

std::wstring integer::to_wstring() const {
    wstringstream ss;
    ss << L"type:" << drawdown::to_wstring(type) << ",value:" << value;
    return ss.str();
}

std::wstring real::to_wstring() const {
    wstringstream ss;
    ss << L"type:" << drawdown::to_wstring(type) << ",value:" << value;
    return ss.str();
}

const std::vector<shared_ptr<token>> &token_builder::get_list() const {
    if (!is_parsed) {
        parse();
        is_parsed = true;
    }
    return list;
}

void token_builder::parse() const {
    for (const wchar_t *it = text.c_str(); *it != L'\0';) {
        if (parse_text(it)){
            continue;
        }else if ( parse_numeral(it) ) {
            continue;
        }else if  ( parse_op(it)){
            continue;
        }else if (parse_bracket(it)){
            continue;
        }
        if (*it == L'.') {
            list.emplace_back(new token(token_type::dot));
            it++;
            continue;
        }
        // operator
        if (*it == L';') {
            list.emplace_back(new token(token_type::line_end));
            it++;
            continue;
        }
        if (*it == L'\n') {
            list.emplace_back(new token(token_type::newline));
            it++;
            continue;
        }
        if (*it==L' '||*it==L'\r'){
            it++;
            continue;
        }

        wcout<<L"Error:"<<hex<<(int)*it<<endl;
        list.emplace_back(new token(token_type::unknown));
        it++;
    }
    list.emplace_back(new token(token_type::end));
}

bool token_builder::parse_numeral(const wchar_t *&it) const {
    //読み込みに使う関数を定義
    auto load_integer = [](const wchar_t *&kt) {
        int result{0};
        for (; iswdigit(*kt); kt++) {
            result = result * 10 + (int)(*kt - L'0');
        }
        return result;
    };
    auto load_point = [](const wchar_t *&kt) -> double {
        double result = 0;
        for (double digit = 0.1; iswdigit(*kt); kt++, digit /= 10) {
            result += digit * (int)(*kt - L'0');
        }
        return result;
    };
    //対象か?
    const wchar_t *jt{it};
    if (!iswdigit(*jt)) {
        return false;
    }

    const int number{load_integer(jt)};
    // is integer ?
    if (*jt != L'.' && *jt != L'e' && *jt != 'E') {
        list.emplace_back(new integer(number));
        it = jt;
        return true;
    };
    // load point
    double result = number;
    if (*jt == L'.') {
        jt++;
        result += load_point(jt);
    }
    if (*jt == L'e' || *jt == L'E') {
        jt++;
        result *= exp10(load_integer(jt));
    }
    list.emplace_back(new real(result));
    it = jt;
    return true;
}

bool token_builder::parse_op(const wchar_t *&it) const {
    if (*it == L'=') {
        list.emplace_back(new token(token_type::set));
        it++;
        return true;
    }
    if (*it == L'+') {
        list.emplace_back(new token(token_type::add));
        it++;
        return true;
    }
    if (*it == L'-') {
        list.emplace_back(new token(token_type::sub));
        it++;
        return true;
    }
    if (*it == L'*') {
        it++;
        if (*it == L'*') {
            it++;
            list.emplace_back(new token(token_type::exp));
        } else {
            list.emplace_back(new token(token_type::mul));
        }
        return true;
    }
    if (*it == L'/') {
        it++;
        if (*it == L'/') {
            it++;
            list.emplace_back(new token(token_type::para));
        } else {
            list.emplace_back(new token(token_type::div));
        }
        return true;
    }
    return false;
}

bool token_builder::parse_bracket(const wchar_t *&it) const {
    if (*it == L'(') {
        list.emplace_back(new token(token_type::bracket_begin));
        it++;
        return true;
    }
    if (*it == L')') {
        list.emplace_back(new token(token_type::bracket_end));
        it++;
        return true;
    }

    if (*it == L'{') {
        list.emplace_back(new token(token_type::block_begin));
        it++;
        return true;
    }
    if (*it == L'}') {
        list.emplace_back(new token(token_type::block_end));
        it++;
        return true;
    }
    return false;
}

bool token_builder::parse_text(const wchar_t *&it) const {
    // label
    if (!iswalpha(*it)) {
        return false;
    }

    const wchar_t *begin = it;
    for (it++; iswalnum(*it); it++) {}
    wstring value(begin, it);
    if (value == L"frame") {
        list.emplace_back(new token(token_type::keyword_frame));
    } else {
        list.emplace_back(new label(value));
    }

    return true;
}

} // namespace drawdown
