#include "format.hpp"
#include <cwctype>
#include <sstream>
using namespace std;
namespace drawdown {

charactor_type get_type(wchar_t c) {
  auto in = [](wchar_t x, const wstring &text) {
    return text.find(x) != wstring::npos;
  };

  if (iswspace(c) || iswcntrl(c)) {
    return charactor_type::space;
  } else if (in(c, L"+-*/=~^\\?@:;.#!$%&'|")) {
    return charactor_type::mark;
  } else if (in(c, L"(){}\"[]\'")) {
    return charactor_type::bracket;
  } else {
    return charactor_type::label;
  }
}

std::vector<std::wstring> load_words(const std::wstring &sprict) {
  std::vector<std::wstring> result;
  wstring buf;
  if (sprict.length() > 0) {
    buf += sprict[0];
  }
  for (int idx = 1; idx < sprict.length(); idx++) {
    const wchar_t last = sprict[idx - 1], now = sprict[idx];
    if (get_type(last) != get_type(now)&&buf.length()>0) {
      result.emplace_back(buf);
      buf.clear();
    }
    if (get_type(now) != charactor_type::space) {
      buf += now;
    }
  }

  return result;
}
} // namespace drawdown
