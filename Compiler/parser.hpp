#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include "stmt.hpp"
#include <exception>
#include <typeinfo>

class Parser {
private:
    class ParseError: public std::exception{};
    int current = 0;
    std::vector<Token>tokens;

    bool check(TokenType type);
    bool match(std::initializer_list<TokenType> tokentypes);
    Token consume(TokenType type,std::string message);
    void error(Token token,std::string message){
        Lexer::error(token,message);
        throw ParseError();
    }
    Token advance();
    bool isAtEnd();
    Token peek();
    Token previous();
    Expr* expression();
    Expr* assignment();
    Expr* equality();
    Expr* comparision();
    Expr* term();
    Expr* factor();
    Expr* unary();
    Expr* primary();
    Expr* orExpr();
    Expr* andExpr();
    Expr* call();
    Expr* finishCall(Expr* callee);
    std::vector<Stmt*> block();
    Stmt* statement();
    Stmt* forStatement();
    Stmt* ifStatement();
    Stmt* printStatement();
    Stmt* returnStatement();
    Stmt* whileStatement();
    Stmt* exprStatement();
    Stmt* declaration();
    Stmt* varDeclaration();
    Stmt* function(std::string kind);

public:
    Parser(const std::vector<Token> &tokens): tokens(tokens) {};
    std::vector<Stmt*> parse();
};

#endif
