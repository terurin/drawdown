#pragma once
#ifndef __AST_HEADER_GUARD__
#define __AST_HEADER_GUARD_
#include "token.hpp"
#include <memory>
namespace drawdown ::ast{

struct ast;
using ast_ptr = std::shared_ptr<ast>;
class ast_builder final {
    const std::vector<token::token_ptr> tokens;
    using token_iterator = std::vector<token::token_ptr>::const_iterator;
    mutable std::shared_ptr<ast> root{
        nullptr}; //遅延生成(コンストラクタでエラーを出さないため)

  public:
    ast_builder(const std::vector<std::shared_ptr<token::token>> &_tokens)
        : tokens(_tokens) {}
    ast_builder(const ast &) = delete;
    ~ast_builder() = default;
    const ast_ptr get_ast() const;

  private:
    ast_ptr build() const;
    ast_ptr parse( token_iterator &it) const;
    ast_ptr parse_frame(token_iterator& it)const;
    ast_ptr parse_set( token_iterator &it) const;
    ast_ptr parse_add_sub(token_iterator &it) const;
    ast_ptr parse_mul_div(token_iterator &it)const;
    ast_ptr parse_exp_para(token_iterator &it)const;
    ast_ptr parse_tail( token_iterator &it) const;
};

enum class ast_type { unknown, add, sub, mul, div,exp,para, set,integer, label ,bracket,frame};

std::wstring to_wstring(ast_type);

struct ast {
    const ast_type type{ast_type::unknown};
    const ast_ptr right, left;
    ast(ast_type _type, ast_ptr _right = nullptr, ast_ptr _left = nullptr)
        : type(_type), right(_right), left(_left) {}
    ast(const ast &) = default;
    virtual ~ast() = default;
    virtual std::wstring to_wstring() const; // LISP形式で出力する
};

struct ast_label : public ast {
    const std::wstring label;
    ast_label(const std::wstring &_label)
        : ast(ast_type::label), label(_label) {}
    ast_label(const ast_label &) = default;
    virtual ~ast_label() = default;
    virtual std::wstring to_wstring() const; // LISP形式で出力する
};

struct ast_int : public ast {
    const int value;
    ast_int(int _value) : ast(ast_type::integer), value(_value) {}
    ast_int(const ast_int &) = default;
    virtual ~ast_int() = default;
    virtual std::wstring to_wstring() const; // LISP形式で出力する
};

struct ast_frame:public ast{
    const std::wstring name;
    ast_frame(const std::wstring& _name,ast_ptr _right):
    ast(ast_type::frame,_right),name(_name){}
    ast_frame(const ast_frame&)=default;
    virtual ~ast_frame()=default;
    virtual std::wstring to_wstring() const; // LISP形式で出力する
};



} // namespace drawdown

#endif