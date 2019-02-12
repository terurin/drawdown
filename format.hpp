#pragma once
#ifndef __FORMAT_HEADER_GUARD__
#define __FORMAT_HEADER_GUARD__
#include <wchar.h>
#include <string>
#include <vector>
#include <fstream>

namespace drawdown {


enum class charactor_type{
    space,//非表示文字
    mark,//演算用記号
    bracket,//括弧記号
    label,//数字及び文字    
};
charactor_type get_type(wchar_t);

std::vector<std::wstring> load_words(const std::wstring&);



} // namespace drawdown

#endif