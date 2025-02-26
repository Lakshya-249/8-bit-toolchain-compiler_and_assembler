#include "stmt.hpp"

Expression::Expression(Expr* expression) {
	this->expression = expression;
}

void Expression::printfunc() {
	std::cout<< "Expression of class"<<std::endl;}

std::string Expression::accept(StmtVisitor* visitor) { return visitor->visitExpression(this); }

Print::Print(Expr* expression) {
	this->expression = expression;
}

void Print::printfunc() {
	std::cout<< "Print of class"<<std::endl;}

std::string Print::accept(StmtVisitor* visitor) { return visitor->visitPrint(this); }

Var::Var(Token name, Expr* initializer) {
	this->name = name;
	this->initializer = initializer;
}

void Var::printfunc() {
	std::cout<< "Var of class"<<std::endl;}

std::string Var::accept(StmtVisitor* visitor) { return visitor->visitVar(this); }
