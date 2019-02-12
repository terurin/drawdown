#include "format.hpp"
#include <cwctype>
#include <sstream>
#include <stdlib.h>
using namespace std;
namespace drawdown {

std::wstring to_wstring(word_type word) {
    switch (word) {
    case word_type::label: return L"label";
    case word_type::end: return L"end";
    default: return L"unknown";
    }
}

std::vector<word*> parse_words(const std::wstring &plain) {
    std::vector<word*> result;

    for (const wchar_t *it = plain.c_str(); *it != L'\0'; it++) {
        if (iswspace(*it)) {
            continue;
        }
        // label
        if (iswalpha(*it)) {
            const wchar_t *end;
            for (end = it; iswprint(*end); end++) {}
            wstring value(it,end);
            result.emplace_back(new label(value));
            continue;
        }
    }

    return result;
}

} // namespace drawdown
