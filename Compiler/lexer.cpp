#include "lexer.hpp"
#include <cctype>

std::unordered_map<std::string, TokenType> Lexer::reservedWordsMap = {
    {"for", FOR}, {"or", OR_OP}, {"if", IF}, {"else", ELSE},
    {"return", RETURN}, {"print", PRINT}, {"int", INT}, {"str", STR},
    {"true", TRUE}, {"false", FALSE}, {"nil", NIL}, {"and", AND_OP}, {"while", WHILE}, 
    {"def", DEF}, 
};

std::string Lexer::stripComment(const std::string& line) {
    size_t commentPos = line.find('#');  
    return (commentPos != std::string::npos) ? line.substr(0, commentPos) : line;  
}

bool Lexer::hadError = false;

bool Lexer::isInteger(const std::string& str) {
    for (char c : str) {
        if (!isdigit(c)) return false;
    }
    return true;
}

void Lexer::report(int line, std::string where, std::string message) {
    std::cerr << "[line " << line << "] Error: " << where << ": " << message << std::endl;
    std::exit(1);
}

void Lexer::error(Token token,std::string message){
    if (token.type == EOFL){
        report(token.line," at end",message);
    }
    else{
        report(token.line," at '" + token.lexeme + "'",message);
    }
}

Token Lexer::createToken(TokenType type, std::string lexeme, int line, int column) {
    return Token(type, lexeme, line, column + 1);
}

Token Lexer::checkLiteralOrKeyword(std::string lexeme, int line, int column) {
    if (reservedWordsMap.find(lexeme) != reservedWordsMap.end()) {
        return createToken(reservedWordsMap[lexeme], lexeme, line, column);
    } else if (isInteger(lexeme)) {
        return createToken(INTEGER, lexeme, line, column);
    } else if (lexeme.front() == '"' && lexeme.back() == '"') {
        return createToken(STRING, lexeme.substr(1, lexeme.size() - 2), line, column);
    }
    return createToken(IDENTIFIER, lexeme, line, column);
}

bool Lexer::match(char c1, char c2,int &i) {
    if (c1 == c2){
        i++;
        return true;
    }
    return false;
};

std::vector<Token> Lexer::tokenize(std::ifstream& file) {
    std::vector<Token> tokens;
    std::string line;
    int lineNumber = 1;
    
    while (std::getline(file, line)) { 
        line = stripComment(line);
        int n = line.size();
        std::string currentToken;
        
        for (int i = 0; i < n; i++) {
            char c = line[i];
            
            if (std::string(",;(){}+-*/><:=!&|^").find(c) != std::string::npos) {
                if (!currentToken.empty()) {
                    tokens.push_back(checkLiteralOrKeyword(currentToken, lineNumber, i));
                    currentToken.clear();
                }
                switch (c)
                {
                case ',': tokens.push_back(createToken(COMMA,",",lineNumber,i)); break;
                case ';': tokens.push_back(createToken(SEMICOLON,";",lineNumber,i)); break;
                case '(': tokens.push_back(createToken(LPAREN,"(",lineNumber,i)); break;
                case ')': tokens.push_back(createToken(RPAREN,")",lineNumber,i)); break;
                case '{': tokens.push_back(createToken(LBRACE,"{",lineNumber,i)); break;
                case '}': tokens.push_back(createToken(RBRACE,"}",lineNumber,i)); break;
                case '+': tokens.push_back(createToken(PLUS,"+",lineNumber,i)); break;
                case '-': tokens.push_back(createToken(MINUS,"-",lineNumber,i)); break;
                case '*': tokens.push_back(createToken(STAR,"*",lineNumber,i)); break;
                case '/': tokens.push_back(createToken(SLASH,"/",lineNumber,i)); break;
                case '|': tokens.push_back(createToken(OR,"|",lineNumber,i)); break;
                case '&': tokens.push_back(createToken(AND,"&",lineNumber,i)); break;
                case '^': tokens.push_back(createToken(XOR,"^",lineNumber,i)); break;
                case '>': tokens.push_back(match('=', line[i+1], i) ? createToken(GREATER_EQUAL,">=",lineNumber,i) : 
                        createToken(GREATER,">",lineNumber,i)); break;
                case '<': tokens.push_back(match('=', line[i+1], i) ? createToken(LESS_EQUAL,"<=",lineNumber,i) : 
                        createToken(LESS,"<",lineNumber,i)); break;
                case ':': tokens.push_back(createToken(COLON,":",lineNumber,i)); break;
                case '=': tokens.push_back(match('=', line[i+1], i) ? createToken(EQUAL_EQUAL,"==",lineNumber,i) : 
                        createToken(EQUAL,"=",lineNumber,i)); break;
                case '!': tokens.push_back(match('=', line[i+1], i) ? createToken(NOT_EQUAL,"!=",lineNumber,i) : 
                        createToken(NOT,"!",lineNumber,i)); break;
                default:
                    tokens.push_back(createToken(static_cast<TokenType>(c), std::string(1, c), lineNumber, i));
                    break;
                }
                
            } else if (isspace(c)) {
                if (!currentToken.empty()) {
                    tokens.push_back(checkLiteralOrKeyword(currentToken, lineNumber, i));
                    currentToken.clear();
                }
            } else {
                currentToken += c;
            }
        }
        
        if (!currentToken.empty()) {
            tokens.push_back(checkLiteralOrKeyword(currentToken, lineNumber, 0));
        }
        
        lineNumber++;
    }
    Token t = Token(EOFL,"eofl",lineNumber,-1);
    tokens.push_back(t);  // Add EOF token to indicate end of file.
    
    return tokens;
}
