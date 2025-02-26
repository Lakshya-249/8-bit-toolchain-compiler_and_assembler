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

Expr* Parser::expression() {
    return assignment();
}

Expr* Parser::assignment() {
    Expr* expr = equality();
    if(match({EQUAL})){
        Token equals = previous();
        Expr* value = assignment();
        if(typeid(*expr) == typeid(Variable)){
            Token name = ((Variable*)expr)->name;
            return new Assign(name, value);
        }
        error(equals, "Invalid assignment target");
        return nullptr;
    }
    return expr;
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
    while (match({LESS, GREATER})){
        Token op = previous();
        Expr* right = term();
        expr = new Binary(expr, op, right);
    }
    return expr;
}

Expr* Parser::term(){
    Expr* expr = factor();
    while (match({PLUS, MINUS})){
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

    if (match({IDENTIFIER})) return new Variable(previous());

    if (match({LPAREN})){
        Expr* expr = expression();
        consume(RPAREN, "Expect ')' after expression");
        return expr;
    }
    // error(peek(), "Expect expression.");
    return nullptr;
}

Stmt* Parser::statement() {
    if (match({PRINT})) return printStatement();
    return exprStatement();
}

Stmt* Parser::printStatement() {
    Expr* expr = expression();
    consume(SEMICOLON, "Expect ';' after value");
    return new Print(expr);
}

Stmt* Parser::exprStatement() {
    Expr* expr = expression();
    consume(SEMICOLON, "Expect ';' after expression");
    return new Expression(expr);
}

Stmt* Parser::declaration() {
    try{
        if (match({INT})) return varDeclaration();
        return statement();
    }
    catch(const Parser::ParseError &e)
    {
        std::cerr << "Oops an error occured!.." << '\n';
        return nullptr;
    }
    
}

Stmt* Parser::varDeclaration() {
    Token name = consume(IDENTIFIER, "Expect variable name");
    Expr* initializer = nullptr;
    if (match({EQUAL})) {
        initializer = expression();
    }
    consume(SEMICOLON, "Expect ';' after declaration");
    return new Var(name, initializer);
}

std::vector<Stmt*> Parser::parse() {
    try
    {
        std::vector<Stmt*> statements;
        while (!isAtEnd())
        {
            statements.push_back(declaration());
        }
        return statements;
    }
    catch(const Parser::ParseError &e)
    {
        std::cerr << "Oops an error occured!.." << '\n';
        return {};
    }
    
}