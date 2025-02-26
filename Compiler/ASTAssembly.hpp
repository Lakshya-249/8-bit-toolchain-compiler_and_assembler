#ifndef ASTASSEMBLY_HPP
#define ASTASSEMBLY_HPP

#include "parser.hpp"
#include <sstream>
#include "environment.hpp"


class Compiler: public ExprVisitor, public StmtVisitor{
    private: 
        Environment env;
        std::ostringstream instructions;
        bool muldataTag = false;
        bool divdataTag = false;
        int mulCount = 0;
        int divCount = 0;

        void pushToStack(char reg);
        void popToRegister(char reg);
        void moveBtwRegister(char reg1,char reg2);
        void moveMemoryToRegister(char reg, std::string memadd);
        void moveRegisterToMemory(char reg, std::string memadd);
        void loadDataRegister(char reg, std::string data);
        std::string getOpCode(const std::string &op);
        std::string evaluate(Expr* expr);
        void multiply(Binary* binary);
        void divide(Binary* binary, std::string opCode);
        void evalAddSub(Binary* binary, std::string opCode);
        std::string visitBinary(Binary* binary) override;
        std::string visitGrouping(Grouping* grouping) override;
        std::string visitLiteral(Literal* literal) override;
        std::string visitUnary(Unary* unary) override;
        std::string visitVariable(Variable* variable) override;
        std::string visitExpression(Expression* expression) override;
        std::string visitPrint(Print* print) override;
        std::string visitVar(Var* variable) override;
        void execute(Stmt* stmt);
    public:
        std::string compile(std::vector<Stmt*> &statements);
        
};

#endif