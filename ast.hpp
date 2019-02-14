#pragma once
#ifndef __AST_HEADER_GUARD__
#define __AST_HEADER_GUARD_
#include "token.hpp"
#include <memory>
namespace drawdown {

struct ast;
using ast_ptr = std::shared_ptr<ast>;

class ast_builder final {
    const std::vector<token_ptr> tokens;
    using token_iterator = std::vector<token_ptr>::const_iterator;
    mutable std::shared_ptr<ast> root{
        nullptr}; //遅延生成(コンストラクタでエラーを出さないため)

  public:
    ast_builder(const std::vector<std::shared_ptr<token>> &_tokens)
        : tokens(_tokens) {}
    ast_builder(const ast &) = delete;
    ~ast_builder() = default;
    ast_ptr get_ast() const;

  private:
    ast_ptr build() const;
    ast_ptr parse(const token_iterator &it) const;
    ast_ptr parse_add(const token_iterator &it) const;
    ast_ptr parse_tail(const token_iterator &it) const;
};

enum class ast_type { unknown,add, sub, mul, div, integer, label };

std::wstring to_wstring(ast_type);

struct ast {
    ast_type type{ast_type::unknown};
    std::shared_ptr<ast> right{nullptr}, left{nullptr};
    ast(ast_type _type) :type(_type){}
    ast(const ast &) = default;
    virtual std::wstring to_wstring() const;//LISP形式で出力する
};

} // namespace drawdown

#endif