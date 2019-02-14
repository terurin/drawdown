#pragma once
#ifndef __TOKEN_HEADER_GUARD__
#define __TOKEN_HEADER_GUARD__
#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <wchar.h>
namespace drawdown {

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
    token(token_type _type) : type(_type) {}
    token(const token &) = default;
    virtual ~token() = default;
    virtual std::wstring to_wstring() const;
    virtual bool operator==(const token &) const;
    bool operator!=(const token &cmp) { return !(*this == cmp); }
};

struct label : public token {
    std::wstring value;
    label(const std::wstring &_value) : token(token_type::label), value(_value) {}
    virtual std::wstring to_wstring() const;
};

struct integer : public token {
    int value;
    integer(int _value) : token(token_type::integer), value(_value) {}
    virtual std::wstring to_wstring() const;
};

struct real : public token {
    double value;
    real(double _value) : token(token_type::real), value(_value) {}
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
};

} // namespace drawdown

#endif