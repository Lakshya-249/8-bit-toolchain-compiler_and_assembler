#include "parser.hpp"


Token Parser::consume(TokenType type,std::string message){
    if (!check(type)) {
        error(peek(),message);
    }
    return advance();
}

bool Parser::isAtEnd(){
    // if (peek().type == TokenType::EOFL){
    //     std::cout<<"isAtEnd"<<std::endl;
    // }
    return peek().type == TokenType::EOFL;
}

bool Parser::check(TokenType &type){
    if (isAtEnd()) {
        // std::cout<<"this is false check"<<std::endl;
        return false;
    }
    return peek().type == type;
} 

Token Parser::peek(){
    return tokens[current];
}

Token Parser::previous(){
    if (current == 0) return Token(EOFL, "", -1, -1);
    return tokens[current - 1];
}

Token Parser::advance(){
    if(!isAtEnd()) current++;
    return previous();
}

bool Parser::match(std::initializer_list<TokenType> tokentypes){
    for(TokenType t: tokentypes){
        if(check(t)){
            advance();
            return true;
        }
    }
    return false;
}

Expr* Parser::expression(){
    return equality();
}

Expr* Parser::equality() {
    Expr* expr = comparision();
    while (match({NOT_EQUAL, EQUAL_EQUAL})) {
        Token op = previous();
        Expr* right = comparision();
        expr = new Binary(expr, op, right);
    }
    return expr;
}

Expr* Parser::comparision(){
    Expr* expr = term();
    while(match({LESS, GREATER})){
        Token op = previous();
        Expr* right = term();
        expr = new Binary(expr, op, right);
    }
    return expr;
}

Expr* Parser::term(){
    Expr* expr = factor();
    while(match({PLUS, MINUS})){
        Token op = previous();
        Expr* right = factor();
        expr = new Binary(expr, op, right);
    }
    return expr;
}

Expr* Parser::factor(){
    Expr* expr = unary();
    while (match({SLASH,STAR})){
        Token op = previous();
        Expr* right = unary();
        expr = new Binary(expr, op, right);
    }
    return expr;
}

Expr* Parser::unary(){
    if (match({NOT_EQUAL,MINUS,PLUS})){
        Token op = previous();
        Expr* right = unary();
        return new Unary(op, right);
    }
    return primary();
}

Expr* Parser::primary(){
    if (match({FALSE})) return new Literal(false);
    if (match({TRUE})) return new Literal(true);
    if (match({NIL})) return new Literal(nullptr);

    if (match({INTEGER, STRING})) return new Literal(previous().lexeme);

    if (match({LPAREN})){
        Expr* expr = expression();
        consume(RPAREN, "Expect ')' after expression");
        return expr;
    }
    error(peek(), "Expect expression.");
    return nullptr;
}

Expr* Parser::parse(){
    try
    {
       return expression();
    }
    catch(const Parser::ParseError &e)
    {
        std::cerr << "Oops an error occured!.." << '\n';
        return nullptr;
    }
    
}