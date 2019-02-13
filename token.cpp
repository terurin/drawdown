#include "token.hpp"
#include <cwctype>
#include <math.h>
#include <sstream>
#include <stdlib.h>
namespace drawdown {
using namespace std;
std::wstring to_wstring(word_type word) {
    switch (word) {
    case word_type::label: return L"label";
    case word_type::integer: return L"integer";
    case word_type::real: return L"real";
    case word_type::add: return L"add";
    case word_type::sub: return L"sub";
    case word_type::mul: return L"mul";
    case word_type::exp: return L"exp";
    case word_type::div: return L"div";
    case word_type::para: return L"para";
    case word_type::dot: return L"dot";
    case word_type::bracket_begin: return L"(";
    case word_type::bracket_end: return L")";
    case word_type::block_begin: return L"{";
    case word_type::block_end: return L"}";
    case word_type::newline: return L"newline";
    case word_type::end: return L"end";
    default: return L"unknown";
    }
}

std::wstring word::to_wstring() const {
    wstringstream ss;
    ss << L"type:" << drawdown::to_wstring(type);
    return ss.str();
}

bool word::operator==(const word &cmp) const { return type == cmp.type; }

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

const std::vector<shared_ptr<word>> &tokener::get_list() const {
    if (!is_parsed) {
        parse();
        is_parsed = true;
    }
    return list;
}

void tokener::parse() const {
    for (const wchar_t *it = text.c_str(); *it != L'\0';) {

        // label
        if (iswalpha(*it)) {
            const wchar_t *begin = it;
            for (; iswalnum(*it); it++) {}
            wstring value(begin, it);
            list.emplace_back(new label(value));
            continue;
        }
        if (parse_numeral(it) || parse_op(it) || parse_bracket(it)) {
            continue;
        }

        if (*it == L'.') {
            list.emplace_back(new word(word_type::dot));
            it++;
            continue;
        }
        // operator
        if (*it == L';') {
            list.emplace_back(new word(word_type::line_end));
            it++;
            continue;
        }
        if (*it == L'\n') {
            list.emplace_back(new word(word_type::newline));
            it++;
        }
    }
}

bool tokener::parse_numeral(const wchar_t *&it) const {
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

bool tokener::parse_op(const wchar_t *&it) const {
    if (*it == L'=') {
        list.emplace_back(new word(word_type::set));
        it++;
        return true;
    }
    if (*it == L'+') {
        list.emplace_back(new word(word_type::add));
        it++;
        return true;
    }
    if (*it == L'-') {
        list.emplace_back(new word(word_type::sub));
        it++;
        return true;
    }
    if (*it == L'*') {
        it++;
        if (*it == L'*') {
            it++;
            list.emplace_back(new word(word_type::exp));
        } else {
            list.emplace_back(new word(word_type::mul));
        }
        return true;
    }
    if (*it == L'/') {
        it++;
        if (*it == L'/') {
            it++;
            list.emplace_back(new word(word_type::para));
        } else {
            list.emplace_back(new word(word_type::div));
        }
        return true;
    }
    return false;
}

bool tokener::parse_bracket(const wchar_t *&it) const {
    if (*it == L'(') {
        list.emplace_back(new word(word_type::bracket_begin));
        it++;
        return true;
    }
    if (*it == L')') {
        list.emplace_back(new word(word_type::bracket_end));
        it++;
        return true;
    }

    if (*it == L'{') {
        list.emplace_back(new word(word_type::block_begin));
        it++;
        return true;
    }
    if (*it == L'}') {
        list.emplace_back(new word(word_type::block_end));
        it++;
        return true;
    }
    return false;
}

} // namespace drawdown
