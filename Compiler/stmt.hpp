#ifndef STMT_HPP
#define STMT_HPP

#include "token.h"
#include <variant>
#include "expr.hpp"

using Object = std::variant<int, double, bool, std::string>;
class Expression;
class Print;
class Var;

class StmtVisitor{
public:
	virtual std::string visitExpression(Expression* expression) = 0;
	virtual std::string visitPrint(Print* print) = 0;
	virtual std::string visitVar(Var* var) = 0;

};

class Stmt {
public:
    virtual std::string accept(StmtVisitor* visitor) = 0;
    virtual ~Stmt() = default;
};


class Expression : public Stmt {
public:
    Expr* expression;

    Expression(Expr* expression);
	void printfunc();
	std::string accept(StmtVisitor* visitor);
	~Expression() {}
};

class Print : public Stmt {
public:
    Expr* expression;

    Print(Expr* expression);
	void printfunc();
	std::string accept(StmtVisitor* visitor);
	~Print() {}
};

class Var : public Stmt {
public:
    Token name;
    Expr* initializer;

    Var(Token name, Expr* initializer);
	void printfunc();
	std::string accept(StmtVisitor* visitor);
	~Var() {}
};

#endif