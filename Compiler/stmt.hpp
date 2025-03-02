#ifndef STMT_HPP
#define STMT_HPP

#include "token.h"
#include <variant>
#include "expr.hpp"

using Object = std::variant<int, double, bool, std::string>;
class Block;
class Expression;
class Function;
class IfStmt;
class Print;
class ReturnStmt;
class Var;
class WhileStmt;

class StmtVisitor{
public:
	virtual std::string visitBlock(Block* block) = 0;
	virtual std::string visitExpression(Expression* expression) = 0;
	virtual std::string visitFunction(Function* function) = 0;
	virtual std::string visitIfStmt(IfStmt* ifstmt) = 0;
	virtual std::string visitPrint(Print* print) = 0;
	virtual std::string visitReturnStmt(ReturnStmt* returnstmt) = 0;
	virtual std::string visitVar(Var* var) = 0;
	virtual std::string visitWhileStmt(WhileStmt* whilestmt) = 0;

};

class Stmt {
public:
    virtual std::string accept(StmtVisitor* visitor) = 0;
    virtual ~Stmt() = default;
};


class Block : public Stmt {
public:
    std::vector<Stmt*> statements;

    Block(std::vector<Stmt*> statements);
	void printfunc();
	std::string accept(StmtVisitor* visitor);
	~Block() {}
};

class Expression : public Stmt {
public:
    Expr* expression;

    Expression(Expr* expression);
	void printfunc();
	std::string accept(StmtVisitor* visitor);
	~Expression() {}
};

class Function : public Stmt {
public:
    Token name;
    std::vector<Token> params;
    std::vector<Stmt*> body;

    Function(Token name, std::vector<Token> params, std::vector<Stmt*> body);
	void printfunc();
	std::string accept(StmtVisitor* visitor);
	~Function() {}
};

class IfStmt : public Stmt {
public:
    Expr* condition;
    Stmt* thenBranch;
    Stmt* elseBranch;

    IfStmt(Expr* condition, Stmt* thenBranch, Stmt* elseBranch);
	void printfunc();
	std::string accept(StmtVisitor* visitor);
	~IfStmt() {}
};

class Print : public Stmt {
public:
    Expr* expression;

    Print(Expr* expression);
	void printfunc();
	std::string accept(StmtVisitor* visitor);
	~Print() {}
};

class ReturnStmt : public Stmt {
public:
    Token name;
    Expr* value;

    ReturnStmt(Token name, Expr* value);
	void printfunc();
	std::string accept(StmtVisitor* visitor);
	~ReturnStmt() {}
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

class WhileStmt : public Stmt {
public:
    Expr* condition;
    Stmt* body;

    WhileStmt(Expr* condition, Stmt* body);
	void printfunc();
	std::string accept(StmtVisitor* visitor);
	~WhileStmt() {}
};

#endif