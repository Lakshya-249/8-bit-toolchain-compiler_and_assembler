#ifndef EXPR_HPP
#define EXPR_HPP

class Expr {
public:
    virtual ~Expr() = default;
};

class Binary : public Expr {
public:
    Expr* left;
    Token op;
    Expr* right;

    Binary(Expr* left, Token op, Expr* right);
};

class Grouping : public Expr {
public:
    Expr* expression;

    Grouping(Expr* expression);
};

class Literal : public Expr {
public:
    Object* value;

    Literal(Object* value);
};

class Unary : public Expr {
public:
    Token op;
    Expr* right;

    Unary(Token op, Expr* right);
};

#endif