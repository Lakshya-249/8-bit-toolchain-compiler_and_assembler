#ifndef EXPR_HPP
#define EXPR_HPP

#include "token.h"
#include <variant>
using Object = std::variant<int, double, bool, std::string>;
class Assign;
class Binary;
class Grouping;
class Literal;
class Logical;
class Unary;
class Variable;

class ExprVisitor{
public:
	virtual std::string visitAssign(Assign* assign) = 0;
	virtual std::string visitBinary(Binary* binary) = 0;
	virtual std::string visitGrouping(Grouping* grouping) = 0;
	virtual std::string visitLiteral(Literal* literal) = 0;
	virtual std::string visitLogical(Logical* logical) = 0;
	virtual std::string visitUnary(Unary* unary) = 0;
	virtual std::string visitVariable(Variable* variable) = 0;

};

class Expr {
public:
    virtual std::string accept(ExprVisitor* visitor) = 0;
    virtual ~Expr() = default;
};


class Assign : public Expr {
public:
    Token name;
    Expr* value;

    Assign(Token name, Expr* value);
	void printfunc();
	std::string accept(ExprVisitor* visitor);
	~Assign() {}
};

class Binary : public Expr {
public:
    Expr* left;
    Token op;
    Expr* right;

    Binary(Expr* left, Token op, Expr* right);
	void printfunc();
	std::string accept(ExprVisitor* visitor);
	~Binary() {}
};

class Grouping : public Expr {
public:
    Expr* expression;

    Grouping(Expr* expression);
	void printfunc();
	std::string accept(ExprVisitor* visitor);
	~Grouping() {}
};

class Literal : public Expr {
public:
    Object value;

    Literal(Object value);
	void printfunc();
	std::string accept(ExprVisitor* visitor);
	~Literal() {}
};

class Logical : public Expr {
public:
    Expr* left;
    Token op;
    Expr* right;

    Logical(Expr* left, Token op, Expr* right);
	void printfunc();
	std::string accept(ExprVisitor* visitor);
	~Logical() {}
};

class Unary : public Expr {
public:
    Token op;
    Expr* right;

    Unary(Token op, Expr* right);
	void printfunc();
	std::string accept(ExprVisitor* visitor);
	~Unary() {}
};

class Variable : public Expr {
public:
    Token name;

    Variable(Token name);
	void printfunc();
	std::string accept(ExprVisitor* visitor);
	~Variable() {}
};

#endif