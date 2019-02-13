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

enum class word_type {
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
    end = -1,
    newline=-2,
    unknown = -3
};

std::wstring to_wstring(word_type);

struct word {
    word_type type;
    word(word_type _type) : type(_type) {}
    word(const word &) = default;
    virtual ~word() = default;
    virtual std::wstring to_wstring() const;
    virtual bool operator==(const word &) const;
    bool operator!=(const word &cmp) { return !(*this == cmp); }
};

struct label : public word {
    std::wstring value;
    label(const std::wstring &_value) : word(word_type::label), value(_value) {}
    virtual std::wstring to_wstring() const;
};

struct integer : public word {
    int value;
    integer(int _value) : word(word_type::integer), value(_value) {}
    virtual std::wstring to_wstring() const;
};

struct real : public word {
    double value;
    real(double _value) : word(word_type::real), value(_value) {}
    virtual std::wstring to_wstring() const;
};

class tokener final {
    const std::wstring text;
    mutable std::vector<std::shared_ptr<word>> list;
    mutable bool is_parsed{false};

  public:
    tokener(const std::wstring &_text) : text(_text) {}
    tokener(const tokener &) = delete;
    ~tokener() = default;
    const std::vector<std::shared_ptr<word>> &get_list() const; //遅延実行

  private:
    void parse() const;
    bool parse_numeral(const wchar_t *&pos) const;
    bool parse_op(const wchar_t *&pos) const;
    bool parse_bracket(const wchar_t *&pos) const;
};

} // namespace drawdown

#endif