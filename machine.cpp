#include "machine.hpp"
#include <iostream>
namespace drawdown::machine {

void machine::run() { std::wcout << decode(root); }

double machine::decode(const ast_ptr &it) {
    using namespace ast;
    using namespace std;
    switch (it->type) {
    case ast_type::add: return decode(it->right) + decode(it->left);
    case ast_type::sub: return decode(it->right) - decode(it->left);
    case ast_type::mul: return decode(it->right) * decode(it->left);
    case ast_type::div: return decode(it->right) / decode(it->left);
    case ast_type::bracket: return decode(it->right);
    case ast_type::integer:
        if (auto integer = dynamic_pointer_cast<ast_int>(it)) {
            return integer->value;
        }
    }
}

}; // namespace drawdown::machine