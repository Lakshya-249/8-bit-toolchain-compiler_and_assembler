#ifndef ASTASSEMBLY_HPP
#define ASTASSEMBLY_HPP

#include "parser.hpp"
#include <sstream>
#include "environment.hpp"


class Compiler: public ExprVisitor, public StmtVisitor{
    private: 
        Environment* env = new Environment();
        std::vector<Environment*> scopeStack;
        std::vector<Stmt*> ptrstatements;
        std::ostringstream instructions;
        int mulCount = 0;
        int divCount = 0;
        int ifCount = 0;
        int boolCount = 0;
        int orAndCount = 0;
        int whileCount = 0;

        void pushToStack(char reg);
        void popToRegister(char reg);
        void moveBtwRegister(char reg1,char reg2);
        void moveMemoryToRegister(char reg, std::string memadd);
        void moveRegisterToMemory(char reg, std::string memadd);
        void loadDataRegister(char reg, std::string data);
        std::string getOpCode(const TokenType &op);
        std::string evaluate(Expr* expr);
        void multiply(Binary* binary);
        void divide(Binary* binary, std::string opCode);
        void evalAddSub(Binary* binary, std::string opCode);
        void evalCondition(Binary* binary, std::string opCode);
        std::string visitAssign(Assign* assign) override;
        std::string visitLogical(Logical* logical) override;
        std::string visitBinary(Binary* binary) override;
        std::string visitGrouping(Grouping* grouping) override;
        std::string visitLiteral(Literal* literal) override;
        std::string visitUnary(Unary* unary) override;
        std::string visitVariable(Variable* variable) override;
        std::string visitBlock(Block* block) override;
        void executeBlock(std::vector<Stmt*> statements, Environment* environment);
        std::string visitExpression(Expression* expression) override;
        std::string visitIfStmt(IfStmt* ifstmt) override;
        std::string visitPrint(Print* print) override;
        std::string visitWhileStmt(WhileStmt* whilestmt) override;
        std::string visitVar(Var* variable) override;
        void execute(Stmt* stmt);
    public:
        std::string compile(std::vector<Stmt*> &statements);
        ~Compiler();
};

#endif