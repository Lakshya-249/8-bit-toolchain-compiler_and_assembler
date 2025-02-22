#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include "expr.hpp"
#include <exception>

class Parser {
private:
    class ParseError: public std::exception{};
    int current = 0;
    std::vector<Token>tokens;

    bool check(TokenType &type);
    bool match(std::initializer_list<TokenType> tokentypes);
    Token consume(TokenType type,std::string message);
    [[noreturn]] void error(Token token,std::string message){
        Lexer::error(token,message);
        throw ParseError();
    }
    Token advance();
    bool isAtEnd();
    Token peek();
    Token previous();
    Expr* expression();
    Expr* equality();
    Expr* comparision();
    Expr* term();
    Expr* factor();
    Expr* unary();
    Expr* primary();

public:
    Parser(const std::vector<Token> &tokens): tokens(tokens) {};
    Expr* parse();
};

#endif
