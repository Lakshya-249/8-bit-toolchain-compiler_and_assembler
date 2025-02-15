#ifndef EXPR_HPP
#define EXPR_HPP

#include "token.h"
#include <variant>

using Object = std::variant<int, double, bool, std::string>;

class Binary;
class Grouping;
class Literal;
class Unary;

class Visitor{
public:
	virtual std::string visitBinary(Binary* binary) = 0;
	virtual std::string visitGrouping(Grouping* grouping) = 0;
	virtual std::string visitLiteral(Literal* literal) = 0;
	virtual std::string visitUnary(Unary* unary) = 0;

};

class Expr {
public:
    virtual std::string accept(Visitor* visitor) = 0;
    virtual ~Expr() = default;
};



class Binary : public Expr {
public:
    Expr* left;
    Token op;
    Expr* right;

    Binary(Expr* left, Token op, Expr* right);
	void printfunc();
	std::string accept(Visitor* visitor);
	~Binary() {}
};

class Grouping : public Expr {
public:
    Expr* expression;

    Grouping(Expr* expression);
	void printfunc();
	std::string accept(Visitor* visitor);
	~Grouping() {}
};

class Literal : public Expr {
public:
    Object value;

    Literal(Object value);
	void printfunc();
	std::string accept(Visitor* visitor);
	~Literal() {}
};

class Unary : public Expr {
public:
    Token op;
    Expr* right;

    Unary(Token op, Expr* right);
	void printfunc();
	std::string accept(Visitor* visitor);
	~Unary() {}
};

#endif