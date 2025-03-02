#ifndef CALLABLE_HPP
#define CALLABLE_HPP

#include <vector>
class Compiler;
class Expr;

class ASTCallable {
    public:
    virtual int arity() = 0;
    virtual std::string call(Compiler* compiler, std::vector<Expr*> arguments) = 0;
    virtual ~ASTCallable() {}
};



#endif