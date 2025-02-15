#include "expr.hpp"
#include <memory>
#include <sstream>

class AstPrinter : public Visitor {
public:
    std::string print(Expr* expr) {
        return expr->accept(this); // Return the result of accept as a string
    }

    std::string visitBinary(Binary* binary) override {
        return parenthesize(binary->op.lexeme, binary->left, binary->right);
    }

    std::string visitGrouping(Grouping* grouping) override {
        return parenthesize("group", grouping->expression);
    }

    std::string visitLiteral(Literal* literal) override {
        if (std::holds_alternative<int>(literal->value)) {
            return std::to_string(std::get<int>(literal->value));
        } else if (std::holds_alternative<double>(literal->value)) {
            return std::to_string(std::get<double>(literal->value));
        } else if (std::holds_alternative<bool>(literal->value)) {
            return std::get<bool>(literal->value) ? "true" : "false";
        } else if (std::holds_alternative<std::string>(literal->value)) {
            return std::get<std::string>(literal->value);
        }
        throw std::runtime_error("Unsupported literal type");
    }

    std::string visitUnary(Unary* unary) override {
        return parenthesize(unary->op.lexeme, unary->right);
    }

private:
    std::string parenthesize(const std::string& name, Expr* expr1, Expr* expr2 = nullptr) {
        std::ostringstream out;
        out << "(" << name << " " << expr1->accept(this);
        if (expr2) {
            out << " " << expr2->accept(this);
        }
        out << ")";
        return out.str();
    }
};

int main(){
    Token t = Token(FOR,"for",10,7);
    std::shared_ptr<Expr> bn = std::make_shared<Binary>(nullptr,t,nullptr);
    std::shared_ptr<Expr> grp = std::make_shared<Grouping>(nullptr);
    std::shared_ptr<Expr> lit = std::make_shared<Literal>(10);
    std::shared_ptr<Expr> unr = std::make_shared<Unary>(t,nullptr);
    Expr* expression = new Binary(
        new Unary(
            Token(TokenType::MINUS, "-", 0, 1),
            new Literal(123)),
        Token(TokenType::STAR, "*", 0, 1),
        new Grouping(new Literal(45.67))
    );

    Expr* expression2 = new Binary(
        new Literal(3),
        Token(TokenType::STAR, "+", 0, 1),
        new Literal(4)
    );

    AstPrinter printer;
    std::cout<<printer.print(expression)<<std::endl;
    std::cout<<printer.print(expression2);

    delete expression2; // Free memory
    delete expression; // Free memory
    return 0;
}