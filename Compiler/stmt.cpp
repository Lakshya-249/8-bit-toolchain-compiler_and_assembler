#include "stmt.hpp"

Block::Block(std::vector<Stmt*> statements) {
	this->statements = statements;
}

void Block::printfunc() {
	std::cout<< "Block of class"<<std::endl;}

std::string Block::accept(StmtVisitor* visitor) { return visitor->visitBlock(this); }

Expression::Expression(Expr* expression) {
	this->expression = expression;
}

void Expression::printfunc() {
	std::cout<< "Expression of class"<<std::endl;}

std::string Expression::accept(StmtVisitor* visitor) { return visitor->visitExpression(this); }

Function::Function(Token name, std::vector<Token> params, std::vector<Stmt*> body) {
	this->name = name;
	this->params = params;
	this->body = body;
}

void Function::printfunc() {
	std::cout<< "Function of class"<<std::endl;}

std::string Function::accept(StmtVisitor* visitor) { return visitor->visitFunction(this); }

IfStmt::IfStmt(Expr* condition, Stmt* thenBranch, Stmt* elseBranch) {
	this->condition = condition;
	this->thenBranch = thenBranch;
	this->elseBranch = elseBranch;
}

void IfStmt::printfunc() {
	std::cout<< "IfStmt of class"<<std::endl;}

std::string IfStmt::accept(StmtVisitor* visitor) { return visitor->visitIfStmt(this); }

Print::Print(Expr* expression) {
	this->expression = expression;
}

void Print::printfunc() {
	std::cout<< "Print of class"<<std::endl;}

std::string Print::accept(StmtVisitor* visitor) { return visitor->visitPrint(this); }

ReturnStmt::ReturnStmt(Token name, Expr* value) {
	this->name = name;
	this->value = value;
}

void ReturnStmt::printfunc() {
	std::cout<< "ReturnStmt of class"<<std::endl;}

std::string ReturnStmt::accept(StmtVisitor* visitor) { return visitor->visitReturnStmt(this); }

Var::Var(Token name, Expr* initializer) {
	this->name = name;
	this->initializer = initializer;
}

void Var::printfunc() {
	std::cout<< "Var of class"<<std::endl;}

std::string Var::accept(StmtVisitor* visitor) { return visitor->visitVar(this); }

WhileStmt::WhileStmt(Expr* condition, Stmt* body) {
	this->condition = condition;
	this->body = body;
}

void WhileStmt::printfunc() {
	std::cout<< "WhileStmt of class"<<std::endl;}

std::string WhileStmt::accept(StmtVisitor* visitor) { return visitor->visitWhileStmt(this); }
