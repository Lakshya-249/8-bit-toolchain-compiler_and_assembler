#include "token.h"

Token::Token(TokenType type, std::string lexeme, int line, int column) 
    : type(type), lexeme(lexeme), line(line), column(column) {}

Token::Token(){}

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case COMMA: return "COMMA";
        case SEMICOLON: return "SEMICOLON";
        case LPAREN: return "LPAREN";
        case RPAREN: return "RPAREN";
        case LBRACE: return "LBRACE";
        case RBRACE: return "RBRACE";
        case COLON: return "COLON";
        case EQUAL: return "EQUAL";
        case EQUAL_EQUAL: return "EQUAL_EQUAL";
        case NOT: return "NOT";
        case NOT_EQUAL: return "NOT_EQUAL";
        case GREATER: return "GREATER";
        case LESS: return "LESS";
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case STAR: return "STAR";
        case SLASH: return "SLASH";
        case IDENTIFIER: return "IDENTIFIER";
        case STRING: return "STRING";
        case INTEGER: return "INTEGER";
        case AND: return "AND";
        case OR: return "OR";
        case XOR: return "XOR";
        case FOR: return "FOR";
        case WHILE: return "WHILE";
        case IF: return "IF";
        case ELSE: return "ELSE";
        case RETURN: return "RETURN";
        case PRINT: return "PRINT";
        case INT: return "INT";
        case STR: return "STR";
        case TRUE: return "TRUE";
        case FALSE: return "FALSE";
        case EOFL: return "EOFL";
        case GREATER_EQUAL: return "GREATER_EQUAL";
        case LESS_EQUAL: return "LESS_EQUAL";
        default: return "UNKNOWN";
    }
}
