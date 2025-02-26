#include "ASTAssembly.hpp"
#include <memory>

class AstPrinter : public ExprVisitor {
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

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << argv[1] << "\n";
        return 1;
    }
    std::vector<Token> tokens = Lexer::tokenize(file);
    file.close();

    for (const auto& token : tokens) {
        std::cout << "Token: " << token.lexeme << " , Type: " << tokenTypeToString(token.type) 
                  << " , Line: " << token.line << std::endl;
    }

    // if (Lexer::hadError){
    //     return 1;
    // }

    Parser parser = Parser(tokens);
    std::vector<Stmt*> statements = parser.parse();
    // AstPrinter printer1;
    // std::cout<<printer1.print(expression)<<std::endl;
    Compiler compiler;
    std::cout << compiler.compile(statements) << std::endl;

    return 0;
}
