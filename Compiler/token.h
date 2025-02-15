#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>
#include <unordered_map>

enum TokenType {
    NIL, 
    
    // SEPERATOR token
    COMMA, SEMICOLON, LPAREN, RPAREN, LBRACE,
    RBRACE, COLON,

    // ASSIGNMENT oprator token
    EQUAL, EQUAL_EQUAL,
    NOT, NOT_EQUAL,
    GREATER, LESS,

    // ARITHMETIC operator token
    PLUS, MINUS,
    STAR, SLASH,
    
    // literal token
    IDENTIFIER,STRING,INTEGER,

    // KEYWORD token
    AND, OR, XOR, FOR,WHILE,
    IF, ELSE, RETURN,PRINT,INT,STR,
    TRUE, FALSE, EOFL
};

class Token {
public:
    TokenType type;
    std::string lexeme;
    int line;
    int column;

    Token();
    Token(TokenType type, std::string lexeme, int line, int column);
};

std::string tokenTypeToString(TokenType type);

#endif