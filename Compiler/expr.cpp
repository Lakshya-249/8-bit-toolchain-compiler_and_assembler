#include "expr.hpp"

Binary::Binary(Expr* left, Token op, Expr* right) {
	this->left = left;
	this->op = op;
	this->right = right;
}

void Binary::printfunc() {
	std::cout<< "Binary of class"<<std::endl;}

std::string Binary::accept(ExprVisitor* visitor) { return visitor->visitBinary(this); }

Grouping::Grouping(Expr* expression) {
	this->expression = expression;
}

void Grouping::printfunc() {
	std::cout<< "Grouping of class"<<std::endl;}

std::string Grouping::accept(ExprVisitor* visitor) { return visitor->visitGrouping(this); }

Literal::Literal(Object value) {
	this->value = value;
}

void Literal::printfunc() {
	std::cout<< "Literal of class"<<std::endl;}

std::string Literal::accept(ExprVisitor* visitor) { return visitor->visitLiteral(this); }

Unary::Unary(Token op, Expr* right) {
	this->op = op;
	this->right = right;
}

void Unary::printfunc() {
	std::cout<< "Unary of class"<<std::endl;}

std::string Unary::accept(ExprVisitor* visitor) { return visitor->visitUnary(this); }

Variable::Variable(Token name) {
	this->name = name;
}

void Variable::printfunc() {
	std::cout<< "Variable of class"<<std::endl;}

std::string Variable::accept(ExprVisitor* visitor) { return visitor->visitVariable(this); }
