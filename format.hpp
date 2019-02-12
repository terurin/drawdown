#pragma once
#ifndef __FORMAT_HEADER_GUARD__
#define __FORMAT_HEADER_GUARD__
#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <wchar.h>
namespace drawdown {

enum class word_type { label, end = -1, unknown = -2 };

std::wstring to_wstring(word_type);

struct word {
    word_type type;
    word(word_type _type) : type(_type) {}
};

struct label : public word {
    std::wstring value;
    label(const std::wstring &_value) : word(word_type::label), value(_value) {}
};

std::vector<word *> parse_words(const std::wstring &);

} // namespace drawdown

#endif