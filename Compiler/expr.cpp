#include "expr.hpp" 
#include "token.h"

Binary::Binary(Expr* left, Token op, Expr* right) {
	this->left = left;
	this->op = op;
	this->right = right;
}

void Binary::printfunc() {
	std::cout<< "Binary of class"<<std::endl;}

std::string Binary::accept(Visitor* visitor) { return visitor->visitBinary(this) ;}

Grouping::Grouping(Expr* expression) {
	this->expression = expression;
}

void Grouping::printfunc() {
	std::cout<< "Grouping of class"<<std::endl;}

std::string Grouping::accept(Visitor* visitor) { return visitor->visitGrouping(this) ;}

Literal::Literal(Object value) {
	this->value = value;
}

void Literal::printfunc() {
	std::cout<< "Literal of class"<<std::endl;}

std::string Literal::accept(Visitor* visitor) { return visitor->visitLiteral(this) ;}

Unary::Unary(Token op, Expr* right) {
	this->op = op;
	this->right = right;
}

void Unary::printfunc() {
	std::cout<< "Unary of class"<<std::endl;}

std::string Unary::accept(Visitor* visitor) { return visitor->visitUnary(this) ;}
