#pragma once
#ifndef __TOKEN_HEADER_GUARD__
#define __TOKEN_HEADER_GUARD__
#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <wchar.h>
#include <cstddef>
namespace drawdown ::token{

enum class token_type {
    label, //
    integer,
    real,
    //通常演算子
    set,  // =
    dot, // .
    add,  // +
    sub,  // -
    mul,  // *
    exp,  // **
    div,  // /
    para, // //
    //関係接続子(予約)
    connect,  // ~ // a ~ b
    rconnect, // ~>
    lconnect, // <~
    //カッコ
    bracket_begin, //(
    bracket_end,   //)
    block_begin,//{
    block_end, //}
    line_end,
    end ,
    newline,
    unknown ,
    //keyword
    keyword_frame
};

std::wstring to_wstring(token_type);

struct token {
    token_type type;
    ptrdiff_t pos;
    token(token_type _type,ptrdiff_t _pos=0) : type(_type),pos(_pos)  {}
    token(const token &) = default;
    virtual ~token() = default;
    virtual std::wstring to_wstring() const;
    virtual bool operator==(const token &) const;
    bool operator!=(const token &cmp) { return !(*this == cmp); }
};

//省略名定義
using token_ptr = std::shared_ptr<token>;


struct label : public token {
    std::wstring value;
    label(const std::wstring &_value,ptrdiff_t _pos=0) : token(token_type::label,_pos), value(_value) {}
    virtual std::wstring to_wstring() const;
};

struct integer : public token {
    int value;
    integer(int _value,ptrdiff_t _pos=0) : token(token_type::integer,_pos), value(_value) {}
    virtual std::wstring to_wstring() const;
};

struct real : public token {
    double value;
    real(double _value,ptrdiff_t _pos=0) : token(token_type::real,_pos), value(_value) {}
    virtual std::wstring to_wstring() const;
};

class token_builder final {
    const std::wstring text;
    mutable std::vector<std::shared_ptr<token>> list;
    mutable bool is_parsed{false};

  public:
    token_builder(const std::wstring &_text) : text(_text) {}
    token_builder(const token_builder &) = delete;
    ~token_builder() = default;
    const std::vector<std::shared_ptr<token>> &get_list() const; //遅延実行

  private:
    void parse() const;
    bool parse_numeral(const wchar_t *&pos) const;
    bool parse_op(const wchar_t *&pos) const;
    bool parse_bracket(const wchar_t *&pos) const;
    bool parse_text(const wchar_t*& pos)const;
    ptrdiff_t diff(const wchar_t* a)const{
        return a-text.c_str();
    }
};

} // namespace drawdown

#endif