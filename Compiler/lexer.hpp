#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <vector>
#include <fstream>

class Lexer {
private:
    static void report(int line, std::string where, std::string message);
public:
    static bool hadError;
    
    static void error(Token token,std::string message);
    static std::unordered_map<std::string, TokenType> reservedWordsMap;
    
    static std::string stripComment(const std::string& line);
    static bool isInteger(const std::string& str);
    static Token createToken(TokenType type, std::string lexeme, int line, int column);
    static Token checkLiteralOrKeyword(std::string lexeme, int line, int column);
    static std::vector<Token> tokenize(std::ifstream& file);
    static bool match(char c1, char c2,int &i);
};

#endif