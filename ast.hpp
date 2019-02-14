#pragma once
#ifndef __AST_HEADER_GUARD__
#define __AST_HEADER_GUARD_
#include "token.hpp"
#include <memory>
namespace drawdown {

enum class tree_type{
    add,
    mul,
    value,
};


class ast {
    virtual std::wstring to_wstring() const;
    
};

class ast_builder final{
    const std::vector<std::shared_ptr<token>> tokens;
    public:
    ast_builder(const std::vector<std::shared_ptr<token>>& _tokens):tokens(_tokens){}
    ast_builder(const ast&)=delete;
    ~ast_builder()=default;
    

};



} // namespace drawdown

#endif