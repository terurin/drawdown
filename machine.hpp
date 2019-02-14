#pragma once
#ifndef __MACHINE_HEADER_GUARD__
#define __MACHINE_HEADER_GUARD__
#include "ast.hpp"
#include <stack>
namespace drawdown::machine {
using ast_ptr = drawdown::ast::ast_ptr;

class machine final {
    const ast_ptr root;
  

  public:
    machine(const ast_ptr &_root) : root(_root) {}
    machine(const machine &) = delete;
    ~machine() = default;
    void run(); //デバック用　あとで消す
    private:
    double decode(const ast_ptr& );//デバック用　あとで消す
};

}; // namespace drawdown::machine

#endif