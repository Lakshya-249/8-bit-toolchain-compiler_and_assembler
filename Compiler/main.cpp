#include "parser.hpp"
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

class AstPrinter2 : public Visitor {
    public:
        std::string print(Expr* expr) {
            instructions.str(""); // Clear output buffer
            instructions << ".text\n";
            expr->accept(this);
            instructions << "out 0\n";
            instructions << "hlt\n";
            instructions << ".data\n";

            return instructions.str();
        }
    
        std::string visitBinary(Binary* binary) override {
            std::string opCode = getOpCode(binary->op.lexeme);
            if(opCode == "mul"){
                dataTag = true;
                // binary->left->accept(this);
                binary->right->accept(this);
                pushToStack();
                binary->left->accept(this);
                
                // instructions << "sta %i" << mulCount << "\n";
                moveBtwRegister('C', 'A');
                popToRegisterB();
                instructions << "ldi A 0\n";
                pushToStack();
                instructions << "start" << mulCount << ":\n";
                instructions << "pop A\n";
                
                instructions << "add\n";
                instructions << "push A\n";
                // instructions << "lda %i" << mulCount << "\n";
                moveBtwRegister('A','C');
                instructions << "dec\n";
                // instructions << "sta %i" << mulCount << "\n";
                moveBtwRegister('C','A');
                instructions << "jnz %start" << mulCount << "\n";
                instructions << "pop A\n";
                mulCount++;
            }
            else if (opCode == "div"){
                divDataTag = true;
                
                binary->right->accept(this);
                pushToStack();
                binary->left->accept(this);
                // instructions << "sta %d" << divCount << "\n";
                popToRegisterB();
                pushToStack();
                instructions << "ldi C 0\n";
                instructions << "startD" << divCount << ":\n";
                instructions << "pop A\n";
                // instructions << "lda %d" << divCount << "\n";
                // instructions << "ldi B " << str << "\n";
                // popToRegisterB();
                instructions << "cmp\n";
                pushToStack();
                // instructions << "push B\n";
                instructions << "jc %remainder" << divCount << "\n";
                // instructions << "lda %q" << divCount << "\n";
                moveBtwRegister('A','C');
                instructions << "inc\n";
                moveBtwRegister('C','A');
                // instructions << "sta %q" << divCount << "\n";
                instructions << "pop A\n";
                // instructions << "lda %d" << divCount << "\n";
                // instructions << "ldi B " << str << "\n";
                // popToRegisterB();
                instructions << "sub\n";
                pushToStack();
                // instructions << "sta %d" << divCount << "\n";
                // instructions << "push B\n";
                instructions << "jmp %startD" << divCount << "\n";
                instructions << "remainder" << divCount << ":\n";
                instructions << "pop A\n";
                moveBtwRegister('A','C');
                // instructions << "lda %q" << divCount << "\n";
                // popToRegisterB();
                divCount++;
            }
            else{
                binary->right->accept(this); // Process right operand
                pushToStack();
        
                binary->left->accept(this);  // Process left operand
                popToRegisterB();
        
                // Emit operation
                instructions << opCode << "\n";
            }
            
            return "";
        }
    
        std::string visitLiteral(Literal* literal) override {
            if (std::holds_alternative<std::string>(literal->value)) {
                // std::cout<<"hello world"<<std::endl;
                instructions << "ldi A " << std::get<std::string>(literal->value) << "\n";
            }
            // std::cout<<literal<<std::endl;
            return std::get<std::string>(literal->value);
        }

        std::string visitGrouping(Grouping* grouping) override {
            grouping->expression->accept(this);
            return "";
        }
    
        std::string visitUnary(Unary* unary) override {
            unary->right->accept(this);
            instructions << getOpCode(unary->op.lexeme) << "\n";
            return "";
        }
    
    private:
        std::ostringstream instructions;
        bool dataTag = false;
        bool divDataTag = false;
        int mulCount = 0;
        int divCount = 0;
    
        void pushToStack() {
            instructions << "push A\n";
        }
    
        void popToRegisterB() {
            instructions << "pop B\n";
        }

        void moveBtwRegister(char X,char Y){
            instructions << "mov " << X << " " << Y << "\n";
        }
    
        std::string getOpCode(const std::string& op) {
            if (op == "+") return "add";
            if (op == "-") return "sub";
            if (op == "*") return "mul";
            if (op == "/") return "div";
            return "UNKNOWN_OP";
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
    Expr* expression = parser.parse();
    AstPrinter printer1;
    std::cout<<printer1.print(expression)<<std::endl;
    AstPrinter2 printer;
    std::cout<<printer.print(expression)<<std::endl;
    delete expression; // Free memory

    return 0;
}
